#include <string.h>
#include <stdlib.h>
// Define the RIOT-specific structure and functions that need to be used
#include "msg.h"
#include "shell.h"
#include "xtimer.h"
#include "net/emcute.h" // example if other header need to be used
#include "MQTTClient.h" // Ensure this is the last header file to be included if it is going to be used 

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "client_id"
#define TOPIC "state"
#define QOS 0
#define KEEP_ALIVE_SEC 20

static MQTTClient client;
static Network network;

// Function to initialize MQTT parameters
static void mqtt_init(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 3000, NULL, 0, NULL, 0);

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Network connect failed\n");
        return;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = CLIENT_ID;
    data.keepAliveInterval = KEEP_ALIVE_SEC;
    data.cleansession = 1;

    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("MQTT connect failed\n");
        return;
    }
}

void *mqtt_thread(void *arg) {
    mqtt_init();
    char payload[20] = "work";
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    while (1) {
        int rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != SUCCESS) {
            printf("MQTT publish failed\n");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Start MQTT client as a separate thread
    kernel_pid_t pid = thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack),
                                     THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                     mqtt_thread, NULL, "mqtt_client");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("[Error] Unable to start mqtt client thread\n");
    }
    return 0;
}
