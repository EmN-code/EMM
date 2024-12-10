#include <stdio.h>
#include "simple-udp.h"
#include "mqtt.h"
#include "process.h"
#include "etimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    static uint16_t mid;
    static char *state = "work";
    mqtt_status_t status;

    PROCESS_BEGIN();

    printf("Connecting to MQTT broker at %s:%d...\n", BROKER_IP, BROKER_PORT);
    status = mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60); // Correct the usage
    if (status != MQTT_STATUS_OK) {
        printf("Failed to connect to MQTT broker, status: %d\n", status);
        PROCESS_EXIT();
    }

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_YIELD();
        if (etimer_expired(&periodic_timer)) {
            printf("Publishing state: %s\n", state);

            // Correct the usage
            status = mqtt_publish(&conn, &mid, "state-topic", (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0);
            if (status != MQTT_STATUS_OK) {
                printf("Failed to publish message, status: %d\n", status);
            }

            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}

static void init_mqtt(void) {
    mqtt_register(&conn, &mqtt_publisher_process, "client_id",
                  mqtt_event_callback, 128);
}