#include <stdio.h>
#include <thread>
#include <string.h>
#include <cmath>

#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#include "calculator_interface_nodejs.h"

extern "C" {
    #include "utils.h"
}

static function<void(char*, double[9])> callbackVoltage;
static function<void(char*, uint8_t)> callbackBattery;
static function<void(char*, uint8_t)> callbackState;
static function<void(char*, uint8_t)> callbackButton;

static bool needToExit;

static amqp_connection_state_t conn;

static void sendMessage(amqp_bytes_t message){
    die_on_error(amqp_basic_publish(conn, 1, amqp_cstring_bytes("amq.direct"), amqp_cstring_bytes("plume calculator to ble"), 0, 0, NULL, message), "Publish");
}

static void receiveMessage(void * payload, size_t len){
    amqp_rpc_reply_t ret;
    amqp_envelope_t envelope;
    amqp_frame_t frame;

    amqp_maybe_release_buffers(conn);
    ret = amqp_consume_message(conn, &envelope, NULL, 0);

    if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
        if (AMQP_RESPONSE_LIBRARY_EXCEPTION == ret.reply_type && AMQP_STATUS_UNEXPECTED_STATE == ret.library_error) {
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

void interfaceNodejsGetData()
{
    while(!needToExit){
        char message[7+2+36];
        receiveMessage(&message, 7+2+36);

        char MAC[6];
        for(int i = 0; i < 6; i++){
            MAC[i] = message[i+1];
        }

        if(message[0] == 'v'){ // voltage
            double voltage[9];

            for(int i = 0; i < 9; i++){
                uint32_t value = (uint8_t)message[7+2+4*i+3] + ((uint8_t)message[7+2+4*i+2] << 8);
                value += ((uint8_t)message[7+2+4*i+1] << 16) + ((uint8_t)message[7+2+4*i+0] << 24);

                //printf("%g ", (double)value);

                int sign;

                if(i < 8){
                    sign = ((int)message[7+1] & (0x1 << i)) >> i;
                }
                else{
                    sign = (int)message[7] & 1;
                }

                //printf("%u : %u\n", (int)message[7] & 2, (int)message[7] & 4);

                voltage[i] = ((1-2*sign)*(double)value);
            }

            //printf("\n");

            callbackVoltage(MAC, voltage);
            callbackButton(MAC, (uint8_t)message[7] >> 1);
        }
        else if(message[0] == 'b'){ // battery
            callbackBattery(MAC, message[7]);
        }
        else if(message[0] == 's'){ // state
            callbackState(MAC, message[7]);
        }
    }

    printf("kill thread\n");
}

static thread task;

void interfaceNodejsStart(function<void(char[6], double[9])> _callbackVoltage, function<void(char[6], uint8_t)> _callbackBattery, function<void(char[6], uint8_t)> _callbackState, function<void(char[6], uint8_t)> _callbackButton){
    callbackVoltage = _callbackVoltage;
    callbackBattery = _callbackBattery;
    callbackState = _callbackState;
    callbackButton = _callbackButton;

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

    amqp_queue_declare(conn, 1, amqp_cstring_bytes("plume calculator to ble"), 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");

    amqp_queue_declare(conn, 1, amqp_cstring_bytes("plume ble to calculator"), 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");

    amqp_queue_bind(conn, 1, amqp_cstring_bytes("plume ble to calculator"), amqp_cstring_bytes("amq.direct"), amqp_cstring_bytes("plume ble to calculator"), amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Binding queue");

    amqp_basic_consume(conn, 1, amqp_cstring_bytes("plume ble to calculator"), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

    task = thread(interfaceNodejsGetData);
    // task.detach();

    needToExit = false;
}

void interfaceNodejsSetState(char MAC[6], uint8_t state){
    amqp_bytes_t message;

    char payload[8];
    payload[0] = 's';
    for(int i = 0; i < 6; i++){
        payload[i+1] = MAC[i];
    }
    payload[7] = state;

    message.len = 8;
    message.bytes = payload;
    sendMessage(message);
}

void interfaceNodejsSetEnable(char MAC[6], uint8_t enable){
    amqp_bytes_t message;

    char payload[8];
    payload[0] = 'e';
    for(int i = 0; i < 6; i++){
        payload[i+1] = MAC[i];
    }
    payload[7] = enable;

    message.len = 8;
    message.bytes = payload;
    sendMessage(message);
}

void interfaceNodejsScanDevice(void){
    amqp_bytes_t message;

    message.len = 1;
    message.bytes = (void*)"d";
    sendMessage(message);
}

void interfaceNodejsSetFrequency(char MAC[6], uint8_t frequency){
    amqp_bytes_t message;

    char payload[8];
    payload[0] = 'f';
    for(int i = 0; i < 6; ++i){
        payload[i+1] = MAC[i];
    }
    payload[7] = frequency;

    message.len = 8;
    message.bytes = payload;
    sendMessage(message);
}

void interfaceNodejsSetCalibration(char MAC[6], uint8_t calibration[18]){
    amqp_bytes_t message;

    char payload[7+18];
    payload[0] = 'c';
    for(int i = 0; i < 6; ++i){
        payload[i+1] = MAC[i];
    }

    for(int i = 0; i < 18; ++i){
        payload[i+7] = calibration[i];
    }

    message.len = 7+18;
    message.bytes = payload;
    sendMessage(message);
}

void interfaceNodejsStop(void){
    needToExit = true;
    amqp_bytes_t message;

    message.len = 1;
    message.bytes = (void*)"d";
    sendMessage(message);

    task.join();
}

void interfaceNodejsClose(void){
    die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
    die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
    die_on_error(amqp_destroy_connection(conn), "Ending connection");
}