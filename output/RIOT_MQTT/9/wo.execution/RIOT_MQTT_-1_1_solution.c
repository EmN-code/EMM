#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "mqtt.h"
#include "msg.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_INTERVAL 5
#define CLIENT_ID "esp32_mqtt"

static mqtt_client_t mqtt_client;
static char mqtt_buffer[128];
static msg_t msg_queue[8];

static void on_mqtt_message(const char *topic, const char *message) {
    printf("Received message on topic %s: %s\n", topic, message);
}

static void *mqtt_publisher_thread(void *arg) {
    (void)arg;

    mqtt_client_init(&mqtt_client, msg_queue, sizeof(msg_queue));
    printf("Connecting to MQTT broker at %s:%d...\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    int res = mqtt_connect(&mqtt_client, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_INTERVAL * 2);
    if (res < 0) {
        printf("Could not connect to MQTT broker: %d\n", res);
        return NULL;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        strncpy(mqtt_buffer, "work", sizeof(mqtt_buffer));
        printf("Publishing 'work' to topic %s\n", MQTT_TOPIC);
        res = mqtt_publish(&mqtt_client, 0, MQTT_TOPIC, mqtt_buffer, strlen(mqtt_buffer), 0);

        if (res < 0) {
            printf("Error while publishing: %d\n", res);
        }

        xtimer_sleep(MQTT_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_publisher_thread, NULL, "mqtt_publisher");
    return 0;
}