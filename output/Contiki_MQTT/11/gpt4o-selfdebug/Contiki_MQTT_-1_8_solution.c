#include "mqtt.h"
#include "contiki.h"
#include "dev/leds.h"

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static char *client_id = "mqtt_client_id";
static char *topic = "state";
static struct etimer timer;

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event,
                                void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("MQTT message published\n");
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    static uint16_t msg_id;
    static char payload[] = "work";

    mqtt_register(&conn, &mqtt_publisher_process, client_id, mqtt_event_callback, MQTT_MAX_SEGMENT_SIZE);

    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, MQTT_KEEP_ALIVE_DEFAULT);

    etimer_set(&timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        mqtt_publish(&conn, &msg_id, topic, (uint8_t *)payload, sizeof(payload) - 1, MQTT_QOS_LEVEL_0);

        etimer_reset(&timer);
    }

    PROCESS_END();
}
