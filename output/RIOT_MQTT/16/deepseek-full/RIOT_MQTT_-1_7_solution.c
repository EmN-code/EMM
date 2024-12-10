
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <event/thread.h>
#include <mbox.h>
#include <stddef.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <dht_params.h>
#include <architecture.h>
#include <time_units.h>
#include <net/gcoap.h>
#include <stdint.h>
#include <xtimer.h>
#include <ztimer.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define KEEP_ALIVE 20

static volatile bool connected = false;
static volatile bool message_sent = false;
static volatile bool message_delivered = false;

static void messageArrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data, data->message->payloadlen, data->message->payload);
}

static void deliveryComplete(void* context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    message_delivered = true;
}

static void connectionLost(void* context, char* cause) {
    printf("Connection lost: %s\n", cause);
    connected = false;
}

int main(void) {
    MQTTClient client;
    Network network;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, &network, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = KEEP_ALIVE;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connectionLost, messageArrived, deliveryComplete);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    connected = true;

    while (1) {
        if (connected) {
            pubmsg.payload = (void*)PAYLOAD;
            pubmsg.payloadlen = strlen(PAYLOAD);
            pubmsg.qos = QOS;
            pubmsg.retained = 0;
            message_sent = false;
            message_delivered = false;

            MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);

            while (!message_delivered) {
                MQTTClient_yield();
            }

            printf("Message with token value %d delivered\n", token);
        }

        xtimer_sleep(5);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
