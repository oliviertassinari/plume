#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <cmath>

#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#include "trajectory_simulation.h"

extern "C" {
    #include "utils.h"
}

#define NOISE       1e-8
#define FREQUENCY   200.0

static amqp_connection_state_t conn;

static void sendMessage(amqp_bytes_t message){
    die_on_error(amqp_basic_publish(conn, 1, amqp_cstring_bytes("amq.direct"), amqp_cstring_bytes("plume ble to calculator"), 0, 0, NULL, message), "Publish");
}

static void receiveMessage(void * payload, size_t len){
    amqp_rpc_reply_t ret;
    amqp_envelope_t envelope;
    amqp_frame_t frame;

    amqp_maybe_release_buffers(conn);
    ret = amqp_consume_message(conn, &envelope, NULL, 0);

    if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
        if (AMQP_RESPONSE_LIBRARY_EXCEPTION == ret.reply_type &&
                AMQP_STATUS_UNEXPECTED_STATE == ret.library_error) {
            if (AMQP_STATUS_OK != amqp_simple_wait_frame(conn, &frame)) {
                return;
            }

            if (AMQP_FRAME_METHOD == frame.frame_type) {
                switch (frame.payload.method.id) {
                    case AMQP_BASIC_ACK_METHOD:
                        /* if we've turned publisher confirms on, and we've published a message
                         * here is a message being confirmed
                         */

                        break;
                    case AMQP_BASIC_RETURN_METHOD:
                        /* if a published message couldn't be routed and the mandatory flag was set
                         * this is what would be returned. The message then needs to be read.
                         */
                        {
                            amqp_message_t message;
                            ret = amqp_read_message(conn, frame.channel, &message, 0);
                            if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
                                return;
                            }

                            amqp_destroy_message(&message);
                        }

                        break;

                    case AMQP_CHANNEL_CLOSE_METHOD:
                        /* a channel.close method happens when a channel exception occurs, this
                         * can happen by publishing to an exchange that doesn't exist for example
                         *
                         * In this case you would need to open another channel redeclare any queues
                         * that were declared auto-delete, and restart any consumers that were attached
                         * to the previous channel
                         */
                        return;

                    case AMQP_CONNECTION_CLOSE_METHOD:
                        /* a connection.close method happens when a connection exception occurs,
                         * this can happen by trying to use a channel that isn't open for example.
                         *
                         * In this case the whole connection must be restarted.
                         */
                        return;

                    default:
                        fprintf(stderr ,"An unexpected method was received %d\n", frame.payload.method.id);
                        return;
                }
            }
        }
    }
    else {
        amqp_bytes_t body = envelope.message.body;
        size_t to_copy = size_t (body.len) < len ? size_t (body.len) : len;
        memcpy(payload, body.bytes, to_copy);

        amqp_destroy_envelope(&envelope);
    }
}

static void *taska(void * args){
    (void) args;

    double array_c1[]=
    {   1.0,    0.0,    0.0,
                    0.0,    1.0,    0.0,
                    0.0,    0.0,    1.0};
    // Second calibration matrix
    double array_c2[]=
    {   100,    0.0,    0.0,
                    0.0,    100,   0.0,
                    0.0,    0.0,    100};

    amqp_bytes_t message;

    gsl_matrix_view c1=gsl_matrix_view_array(array_c1,3,3);
    gsl_matrix_view c2=gsl_matrix_view_array(array_c2,3,3);
    gsl_matrix *true_position=gsl_matrix_alloc(3,1);
    gsl_matrix *orientation=gsl_matrix_alloc(3,1);
    gsl_matrix *voltages=gsl_matrix_alloc(3,3);

    TrajectorySimulation *ts=new TrajectorySimulation(3,3,0,0,0,5,0,NOISE,FREQUENCY,&c1.matrix,&c2.matrix);

    while(1){
        ts->next(voltages,true_position,orientation);

        char MAC[7]; // f59f2f10b721
        MAC[0] = 245;
        MAC[1] = 159;
        MAC[2] = 47;
        MAC[3] = 16;
        MAC[4] = 183;
        MAC[5] = 33;
        char payload[7+2+36];

        payload[0] = 'v';

        for(int i = 0; i < 6; i++){
            payload[1+i] = MAC[i];
        }

        payload[7] = 0;
        payload[7+1] = 0;

        // Convert voltage from uint8 to matrix
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                double f = gsl_matrix_get(voltages, i, j)*pow(2,32);
                double fnorm;
                int index = i+3*j;
                int sign;

                if (f < 0){
                    fnorm = -f;
                    sign = 1;
                }
                else {
                    fnorm = f;
                    sign = 0;
                }

                if(index < 8){
                    payload[7+1] |= (sign << index);
                }
                else{
                    payload[7] |= sign;
                }

                //printf("%u : %u\n", 3*i+j, sign);

                uint32_t value = fnorm;
                payload[7+2+4*index+3] = value;
                payload[7+2+4*index+2] = value >> 8;
                payload[7+2+4*index+1] = value >> 16;
                payload[7+2+4*index+0] = value >> 24;

                //printf("%x\n", value);
            }

            printf("%g, %g, %g \n\r", gsl_matrix_get(voltages, i, 0), gsl_matrix_get(voltages, i, 1), gsl_matrix_get(voltages, i, 2));
        }

        printf("----\n");

        message.len = 7+2+36;
        message.bytes = payload;
        sendMessage(message);
        usleep(4000);

        payload[0] = 's';
        payload[7] = 1;
        message.len = 8;
        message.bytes = payload;
        sendMessage(message);
        usleep(4000);

        // Simulate an other plume receiver
        payload[0] = 'b';
        payload[6] = 1;
        payload[7] = 50;
        message.len = 8;
        message.bytes = payload;
        sendMessage(message);
        usleep(4000);

        payload[0] = 's';
        payload[7] = 2;
        message.len = 8;
        message.bytes = payload;
        sendMessage(message);
        usleep(4000);
    }

    return NULL;
}

void signalHandler(int sig){
    (void) sig;

    printf("You pressed Ctrl+C - or killed me with -2\n");

    die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
    die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
    die_on_error(amqp_destroy_connection(conn), "Ending connection");

    exit(0);
}

int main(int argc, const char* argv[]){
    (void) argc;
    (void) argv;

    conn = amqp_new_connection();

    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        die("creating TCP socket");
    }

    int status = amqp_socket_open(socket, "localhost", 5672);
    if (status) {
        die("opening TCP socket");
    }

    die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest"), "Logging in");
    amqp_channel_open(conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

    amqp_queue_declare(conn, 1, amqp_cstring_bytes("plume ble to calculator"), 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");

    amqp_queue_declare(conn, 1, amqp_cstring_bytes("plume calculator to ble"), 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");

    amqp_queue_bind(conn, 1, amqp_cstring_bytes("plume calculator to ble"), amqp_cstring_bytes("amq.direct"), amqp_cstring_bytes("plume calculator to ble"), amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Binding queue");

    amqp_basic_consume(conn, 1, amqp_cstring_bytes("plume calculator to ble"), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

    signal(SIGINT, signalHandler);

    pthread_t ta;
    pthread_create(&ta, NULL, taska, NULL);

    printf("test ok\r\n");

    while(1){
        char message[7+36];
        receiveMessage(&message, 7+36);
        printf("%s\n", message);
    }

    return 0;
}