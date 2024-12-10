#include "xtimer.h"
#include "thread.h"
#include "net/ipv6/addr.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_riot_client"
#define TOPIC "device/state"

#define MESSAGE "work"
#define INTERVAL (5LU * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];

/* Network initialization specifics not included since they depend on setup */

static MQTTClient client;
static Network network;

static void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MESSAGE;
    message.payloadlen = strlen(MESSAGE);
    
    while (1) {
        int rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message, return code %d\n", rc);
        }
        else {
            printf("Message successfully published: %s\n", MESSAGE);
        }
        xtimer_sleep(INTERVAL);
    }
}

int main(void) {
    /* Network and MQTT Client Initialization */
    NetworkInit(&network);
    int rc = ConnectNetwork(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &connect_data);
    if (rc != SUCCESS) {
        printf("Failed to connect to broker, return code %d\n", rc);
        return -1;
    }

    printf("Successfully connected to MQTT broker\n");

    /* Create a thread for publishing */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, (thread_task_func_t)mqtt_publish, NULL, "mqtt_publish");

    /* Main thread will now idle */
    while(1) {
        xtimer_sleep(UINT32_MAX);
    }
    return 0;
}
