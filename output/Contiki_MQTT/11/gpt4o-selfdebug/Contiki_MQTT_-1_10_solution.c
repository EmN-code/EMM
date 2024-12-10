#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>
#include <string.h>

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static char client_id[32];
static char pub_topic[] = "device/state";
static char pub_message[64];
static struct etimer publish_timer;

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    PROCESS_BEGIN();

    snprintf(client_id, sizeof(client_id), "client-%02x%02x", linkaddr_node_addr.u8[6], linkaddr_node_addr.u8[7]);
    mqtt_register(&conn, &mqtt_publisher_process, client_id, NULL, MQTT_MAX_TCP_SEGMENT_SIZE);

    // MQTT Connect properties, refer to mqtt.h in providing these details
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, (60 * CLOCK_SECOND), MQTT_CLEAN_SESSION_ON);

    etimer_set(&publish_timer, 5 * CLOCK_SECOND);

    while(1) {
        PROCESS_YIELD();

        if (ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            strcpy(pub_message, (rand() % 2) == 0 ? "work" : "not work");

            mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}
