#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "mqtt.h"
#include "sys/etimer.h"
#include "random.h"
#include "stdio.h"
#include "string.h"
#include "contiki.h"

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char state_message[12];  // To hold "work" or "not work"

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to the broker successfully.\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from the broker.\n");
            break;
        default:
            printf("Unknown MQTT event: %d\n", event);
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, "nrf52840_client", mqtt_event_handler, 100);
    mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, 60);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        if(random_rand() % 2) {
            strcpy(state_message, "work");
        } else {
            strcpy(state_message, "not work");
        }

        mqtt_publish(&conn, NULL, "/device/state", (uint8_t *)state_message, strlen(state_message), MQTT_QOS_LEVEL_0);

        printf("Published message: %s\n", state_message);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}