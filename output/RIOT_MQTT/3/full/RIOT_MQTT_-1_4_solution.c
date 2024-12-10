#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "string.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

static msg_t _main_msg_queue[8];  // Message queue for handling requests
static char _client_stack[CLIENT_THREAD_STACKSIZE];

void *mqtt_client_thread(void *arg) {
    (void)arg;
    printf("MQTT Client Thread Started\n");

    // Setup network structure
    Network network;
    MQTTClient client;
    unsigned char sendbuf[256], readbuf[256];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Convert server IP for connection
    struct sockaddr_in addr;
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);
    addr.sin_port = htons(SERVER_PORT);

    // Connect to the network
    if (NetworkConnect(&network, SERVER_IP, SERVER_PORT) != 0) {
        printf("Network connection failed\n");
        return NULL;
    }

    // MQTT Connect
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT-ESP32-Client";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("MQTT connection failed\n");
        return NULL;
    }

    printf("MQTT Connected\n");

    // Loop to publish message every five seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = "work";
        message.payloadlen = strlen("work");

        if (MQTTPublish(&client, "state", &message) != 0) {
            printf("MQTT Publish failed\n");
        } else {
            printf("Message published\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, ARRAY_SIZE(_main_msg_queue));
    puts("RIOT MQTT client starting\n");

    // Start the MQTT client thread
    thread_create(_client_stack, sizeof(_client_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_client_thread, NULL, "mqtt_client_thread");

    // Keep the main thread running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}