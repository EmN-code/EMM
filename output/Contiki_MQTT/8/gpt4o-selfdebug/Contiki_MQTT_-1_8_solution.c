#include <stdio.h>
#include "net/mac/mac.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip.h"
#include "mqtt.h"
#include "contiki.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840_client"
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_TOPIC "nrf52840/status"

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    static char *state[] = {"work", "not work"};
    static uint8_t state_index = 0;

    mqtt_register(&conn, &mqtt_publish_process, CLIENT_ID, mqtt_event_callback, 128);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60 * CLOCK_SECOND);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_YIELD();

        if (ev == PROCESS_EVENT_TIMER) {
            if (data == &publish_timer) {
                mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)state[state_index],
                             strlen(state[state_index]), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

                state_index = (state_index + 1) % 2;
                etimer_reset(&publish_timer);
            }
        }
    }

    PROCESS_END();
}

void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT Connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT Disconnected\n");
            process_poll(&mqtt_publish_process);
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Publish complete\n");
            break;
        case MQTT_EVENT_SUBACK:
        case MQTT_EVENT_UNSUBACK:
            break;
        case MQTT_EVENT_PUBACK:
            printf("Publish acknowledged\n");
            break;
        default:
            printf("MQTT event with id: %i\n", event);
            break;
    }
}