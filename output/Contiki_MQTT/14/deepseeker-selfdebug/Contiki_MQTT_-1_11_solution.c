
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "lib/random.h"
#include "simple-udp.h"
#include "mqtt.h"

#define CLIENT_ID "nrf52840_client"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"

static struct mqtt_connection mqtt_conn;
static struct simple_udp_connection udp_conn;
static struct etimer et;

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static char message[20];
    static uint8_t state = 0;

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, UIP_HTONS(BROKER_PORT), NULL, UIP_HTONS(BROKER_PORT), NULL);
    mqtt_register(&mqtt_conn, PROCESS_CURRENT(), CLIENT_ID, mqtt_event_callback, 128);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        state = !state;
        snprintf(message, sizeof(message), "state: %s", state ? "work" : "not work");

        mqtt_publish(&mqtt_conn, TOPIC, message, strlen(message), 0, 0);

        etimer_reset(&et);
    }

    PROCESS_END();
}
