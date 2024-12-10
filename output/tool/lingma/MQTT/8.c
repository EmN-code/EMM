#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/sock/tcp.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PUBLISH_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static char mqtt_stack[MQTT_THREAD_STACKSIZE];
static char sendbuf[128];
static char readbuf[128];

static void *mqtt_thread(void *arg) {
    (void)arg;

    char *broker_ip = MQTT_BROKER_IP;
    int broker_port = MQTT_BROKER_PORT;
    struct mqtt_client client;
    struct mqtt_message msg;
    struct mqtt_topic_subscription sub;

    mqtt_init(&client, NULL, broker_ip, broker_port, 0, NULL, NULL);

    while (1) {
        if (mqtt_is_connected(&client)) {
            msg.qos = MQTT_QOS_0;
            msg.retained = 0;
            msg.dup = 0;
            msg.id = 0;
            msg.payload = "work";
            msg.payload_len = strlen("work");

            mqtt_publish(&client, MQTT_TOPIC, &msg);

            xtimer_sleep(5); // Sleep for 5 seconds
        } else {
            printf("Connecting to MQTT broker...\n");
            mqtt_connect(&client);
            xtimer_sleep(1); // Wait for connection
        }
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT client...\n");

    thread_create(mqtt_stack, sizeof(mqtt_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    while (1) {
        xtimer_sleep(1); // Main loop to keep the program running
    }

    return 0;
}