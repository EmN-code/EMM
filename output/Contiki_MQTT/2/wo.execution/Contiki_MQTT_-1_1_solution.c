#include "mqtt-snc.h"
#include "etimer.h"
#include "simple-udp.h"
#include "contiki-net.h"
#include "contiki.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PERIOD (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer et;

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, "client_id", mqtt_event, 512);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 1000);

    etimer_set(&et, PERIOD);

    while(1) {
        PROCESS_YIELD();
        if(etimer_expired(&et)) {
            static char *msg = "work";
            mqtt_publish(&conn, NULL, "state", (uint8_t *)msg, strlen(msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&et);
        }
    }

    PROCESS_END();
}

void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data)
{
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISHED:
            printf("MQTT message published\n");
            break;
        default:
            printf("Unhandled MQTT event: %d\n", event);
            break;
    }
}