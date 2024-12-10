
#include "contiki.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "string.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "sys/process.h"

#define LOG_MODULE "MQTT App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_KEEP_ALIVE 60

static struct mqtt_connection conn;
static struct etimer et;

static void event_callback(struct mqtt_connection *c, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECT) {
        LOG_INFO("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECT) {
        LOG_INFO("MQTT disconnected\n");
    }
}

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data)
{
    static uint16_t mid;
    static char payload[12];

    PROCESS_BEGIN();

    mqtt_register(&conn, NULL, MQTT_CLIENT_ID, event_callback, 0);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        strcpy(payload, "work");
        mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}
