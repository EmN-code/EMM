#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>
#include <string.h>

PROCESS(mqtt_example, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example);

static struct mqtt_connection conn;

static const char *broker_ip = "47.102.103.1";
#define BROKER_PORT 1883
#define KEEP_ALIVE 60
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5) // 5 seconds

static char *pub_topic = "device/state";
static char *pub_message_on = "work";
static char *pub_message_off = "not work";

PROCESS_THREAD(mqtt_example, ev, data) {
    static struct etimer timer;
    static uint16_t mid = 0;
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example, "nrf52840_client", NULL, MAX_TCP_SEGMENT_SIZE_DEFAULT);
    mqtt_connect(&conn, broker_ip, BROKER_PORT, KEEP_ALIVE, MQTT_CLEAN_SESSION_ON, NULL);

    etimer_set(&timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        mqtt_publish(&conn, &mid, pub_topic, (uint8_t *)pub_message_on, strlen(pub_message_on), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, NULL);

        etimer_reset(&timer);
    }

    PROCESS_END();
}