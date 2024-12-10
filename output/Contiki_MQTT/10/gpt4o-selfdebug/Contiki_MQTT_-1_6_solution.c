#include <mqtt.h>
#include <net/ipv6/uip.h>
#include <net/ipv6/udp-socket.h>
#include <net/ipv6/uip-udp-packet.h>
#include <net/ipv6/simple-udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_connection conn;
static struct process *my_process;

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static char *state = "work or not work";

static void mqtt_event_handler(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISHED:
            printf("Published to MQTT broker\n");
            break;
        default:
            printf("Unhandled event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    mqtt_register(&conn, my_process, "nrf52840_client", mqtt_event_handler, 128);

    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, MQTT_PROP_USE_MEMB);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT();
        if (ev == PROCESS_EVENT_TIMER) {
            etimer_reset(&timer);

            mqtt_publish(&conn, "test/topic", (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        }
    }

    PROCESS_END();
}

int main() {
    process_start(&mqtt_publisher_process, NULL);
    while (1) {
        process_run();
    }
    return 0;
}