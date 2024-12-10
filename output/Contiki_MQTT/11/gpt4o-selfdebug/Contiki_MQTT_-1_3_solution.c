#include "sensors.h"
#include "sdk_config.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-udp-packet.h"
#include "string.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL 5 /* in seconds */
#define MQTT_CLIENT_ID "nrf52840_mqtt_client"
#define MQTT_TOPIC "nrf52840/status"

static struct mqtt_connection conn;
static struct etimer publish_timer;

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("Successfully connected to the broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from the broker\n");
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&publish_timer, CLOCK_SECOND * MQTT_PUBLISH_INTERVAL);

    mqtt_register(&conn, &mqtt_client_process, MQTT_CLIENT_ID, mqtt_event_handler, MQTT_MAX_SEGMENT_SIZE);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE_TIME);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        const char* state = "work";  // or "not work"
        mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}
