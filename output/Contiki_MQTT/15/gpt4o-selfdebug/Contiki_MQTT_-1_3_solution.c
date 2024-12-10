#include "mqtt.h"
#include "contiki.h"
#include "client.h"
#include "dev/leds.h"

#define BROKER_IP_ADDR  "47.102.103.1"
#define BROKER_PORT     1883
#define PUB_TOPIC       "nrf52840/state"
#define STATE_MSG       "work"
#define INTERVAL        (CLOCK_SECOND * 5)

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static struct etimer timer;

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, "nrf52840", NULL, 256);
    mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, 60);

    etimer_set(&timer, INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        mqtt_publish(&conn, NULL, PUB_TOPIC, (uint8_t *)STATE_MSG, strlen(STATE_MSG), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        etimer_reset(&timer);
        leds_toggle(LEDS_ALL);
    }

    PROCESS_END();
}