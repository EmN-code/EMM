#include "process.h"
#include "mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct etimer publish_timer;
static struct mqtt_connection conn;
static uint16_t mid;

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL, MQTT_CLEAN_SESSION_ON);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        const char *topic = "device/state";
        const char *payload = "work";
        mqtt_qos_level_t qos = MQTT_QOS_LEVEL_0;

        mqtt_publish(&conn, &mid, (char *)topic, (uint8_t *)payload, strlen(payload), qos);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}