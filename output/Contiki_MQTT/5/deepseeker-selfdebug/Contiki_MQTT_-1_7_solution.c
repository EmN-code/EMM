
#include <stdio.h>
#include <string.h>
#include "net/mqtt.h"
#include "xtimer.h"
#include "thread.h"

#define BROKER_HOST "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_SIZE 4
#define KEEP_ALIVE 60
#define INTERVAL 5

static struct mqtt_connection conn;
static char stack[THREAD_STACKSIZE_DEFAULT];

void *mqtt_thread(void *arg)
{
    (void)arg;

    while (1) {
        uint16_t mid;
        mqtt_status_t status;

        status = mqtt_publish(&conn, &mid, TOPIC, (uint8_t *)PAYLOAD, PAYLOAD_SIZE, MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
        if (status != MQTT_STATUS_SUCCESS) {
            printf("MQTT publish failed with status %d\n", status);
        } else {
            printf("MQTT publish successful\n");
        }

        xtimer_sleep(INTERVAL);
    }

    return NULL;
}

int main(void)
{
    mqtt_status_t status;

    status = mqtt_register(&conn, process_current(), "nrf52840_client", NULL, 0);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("MQTT register failed with status %d\n", status);
        return 1;
    }

    status = mqtt_connect(&conn, BROKER_HOST, BROKER_PORT, KEEP_ALIVE, MQTT_CLEAN_SESSION_TRUE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("MQTT connect failed with status %d\n", status);
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
