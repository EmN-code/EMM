
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "xtimer.h"
#include "thread.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_SIZE 4
#define INTERVAL 5

static struct mqtt_connection conn;
static char stack[THREAD_STACKSIZE_DEFAULT];

void publish_state(void) {
    uint16_t mid;
    mqtt_status_t status;

    status = mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)PAYLOAD, PAYLOAD_SIZE, MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to publish message: %d\n", status);
    } else {
        printf("Message published successfully\n");
    }
}

void *mqtt_thread(void *arg) {
    (void)arg;

    mqtt_status_t status;

    status = mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, MQTT_CLEAN_SESSION_TRUE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", status);
        return NULL;
    }

    while (1) {
        publish_state();
        xtimer_sleep(INTERVAL);
    }

    mqtt_disconnect(&conn, NULL);
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
