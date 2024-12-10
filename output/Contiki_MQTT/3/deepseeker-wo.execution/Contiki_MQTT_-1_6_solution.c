
#include "contiki.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "string.h"
#include "random.h"
#include "sys/clock.h"
#include "sys/process.h"
#include "mqtt.h"

#define LOG_MODULE "MQTT Example"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_KEEP_ALIVE 60

static struct mqtt_connection conn;
static struct etimer et;

static void mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        LOG_INFO("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        LOG_INFO("MQTT disconnected\n");
    }
}

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
    static uint16_t mid;

    PROCESS_BEGIN();

    mqtt_register(&conn, PROCESS_CURRENT(), "client_id", mqtt_event_handler, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (mqtt_connected(&conn)) {
            mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS_0, 0, 0, 0, NULL);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}
