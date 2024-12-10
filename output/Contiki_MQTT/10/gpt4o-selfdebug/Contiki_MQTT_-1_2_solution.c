#include <mqtt.h>
#include <net/ipv6/multicast.h>
#include <net/ipv6/uiplib.h>
#include <os/sys/etimer.h>

PROCESS(mqtt_example_process, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "work_status"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS_THREAD(mqtt_example_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, "client_id",
                  mqtt_event, MAX_TCP_SEGMENT_SIZE);
    mqtt_set_username_password(&conn, "", "");
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 1000);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER && etimer_expired(&publish_timer)) {
            const char *state = "work"; // Change to "not work" as needed
            mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)state, strlen(state),
                         MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data)
{
    if(event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected to broker\n");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected from broker\n");
    }
}