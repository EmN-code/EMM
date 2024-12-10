#include "gpio-hal.h"
#include "mqtt.h"
#include "net/netstack.h"
#include "contiki.h"
#include "sys/etimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "sensor/state"
#define CLIENT_ID "nrf52840_client"
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char *pub_state = "work";

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static void mqtt_event(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED: 
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker (Reason: %d)\n", *((mqtt_event_t *)data));
            break;
        default:
            printf("MQTT Event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, CLIENT_ID, mqtt_event, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, (CLOCK_SECOND * 60), 0);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        mqtt_publish(&conn, NULL, MQTT_PUB_TOPIC, (uint8_t *)pub_state, strlen(pub_state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        printf("Published state: %s\n", pub_state);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}