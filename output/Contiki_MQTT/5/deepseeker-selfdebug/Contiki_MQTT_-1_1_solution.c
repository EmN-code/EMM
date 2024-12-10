
#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "lib/random.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "mqtt.h"

#define LOG_MODULE "MQTT App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "nrf52840_client"

#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer publish_timer;

static void mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        LOG_INFO("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        LOG_INFO("MQTT disconnected\n");
    }
}

static void publish_state(void) {
    static int state = 0;
    char payload[16];
    snprintf(payload, sizeof(payload), "state: %s", state ? "work" : "not work");
    state = !state;

    mqtt_publish(&conn, MQTT_TOPIC, payload, strlen(payload), 0, 0, NULL, NULL);
    LOG_INFO("Published: %s\n", payload);
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();

    LOG_INFO("Starting MQTT client\n");

    mqtt_init(&conn, MQTT_CLIENT_ID, NULL, NULL, mqtt_event_handler, NULL);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0, 0, NULL, NULL);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        etimer_reset(&publish_timer);

        publish_state();
    }

    PROCESS_END();
}
