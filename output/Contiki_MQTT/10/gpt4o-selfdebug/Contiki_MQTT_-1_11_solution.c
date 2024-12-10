#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/tcpip.h"
#include "mqtt.h"
#include "contiki.h"
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define APP_BUFFER_SIZE 512
#define STATE_PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static char app_buffer[APP_BUFFER_SIZE];
static struct etimer periodic_timer;

PROCESS(mqtt_example_process, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT Connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT Disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("MQTT Publish\n");
            break;
        default:
            printf("MQTT Event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, "myclient", mqtt_event, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, 1883, 100, MQTT_CLEAN_SESSION_ON);

    etimer_set(&periodic_timer, STATE_PUBLISH_INTERVAL);

    while (1) {
        PROCESS_YIELD();

        if (ev == PROCESS_EVENT_TIMER && data == &periodic_timer) {
            snprintf(app_buffer, sizeof(app_buffer), "State: work or not work");
            mqtt_publish(&conn, NULL, "state/topic", app_buffer, strlen(app_buffer), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}

int main() {
    printf("Starting MQTT Example\n");
    process_start(&mqtt_example_process, NULL);
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_EXIT);
    return 0;
}