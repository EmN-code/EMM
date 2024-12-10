#include "sys/etimer.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-udp-packet.h"
#include "os/net/ipv6/mqtt/client.h"
#include "contiki.h"
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_connection mqtt_conn;
static struct etimer et;

PROCESS(mqtt_pub_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_pub_process);

void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("MQTT publish\n");
            break;
        default:
            printf("MQTT event other %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_pub_process, ev, data) {
    static char *state = "work";

    PROCESS_BEGIN();
    
    mqtt_register(&mqtt_conn, &mqtt_pub_process, "nrf52840-client", mqtt_event, 100);
    mqtt_connect(&mqtt_conn, BROKER_IP, BROKER_PORT, 1000);

    etimer_set(&et, 5 * CLOCK_SECOND);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        mqtt_publish(&mqtt_conn, NULL, "state", (uint8_t *)state, strlen(state), 0, MQTT_QOS_LEVEL_0, 0);
        etimer_reset(&et);
    }

    PROCESS_END();
}
