#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#include <iostream>

extern "C" {
    #include "utils.h"
}

#define NOISE       0
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

    amqp_bytes_t message;

    while(1){
        char MAC[7] = "MACMAC";
        char payload[7+36];

        payload[0] = 's';

        for(int i = 0; i < 6; i++){
            payload[1+i] = MAC[i];
        }

        struct timeval start;
        gettimeofday(&start, NULL);
        uint32_t value1 = (uint32_t)start.tv_sec;
        uint32_t value2 = (uint32_t)start.tv_usec;

        printf("%u %u\n\r", value1, value2);

        message.len = 8;
        message.bytes = payload;
        sendMessage(message);
        usleep(100000);
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