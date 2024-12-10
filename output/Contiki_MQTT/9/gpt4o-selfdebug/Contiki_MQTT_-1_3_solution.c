#include <stdio.h>
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"

#define MQTT_KEEP_ALIVE 60
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_state_publisher, "MQTT State Publisher");
AUTOSTART_PROCESSES(&mqtt_state_publisher);

static struct etimer publish_timer;
static struct mqtt_connection conn;

void prepare_mqtt_connection() {
    mqtt_register(&conn, &mqtt_state_publisher, "nrf52840_client", NULL, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, MQTT_KEEP_ALIVE);
}

PROCESS_THREAD(mqtt_state_publisher, ev, data)
{
    PROCESS_BEGIN();

    etimer_set(&publish_timer, PUBLISH_INTERVAL);
    prepare_mqtt_connection();

    while(1) {
        PROCESS_YIELD();

        if (ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            uint16_t mid;
            const char *topic = "device/state";
            const char *msg = "work";
            mqtt_publish(&conn, &mid, topic, (uint8_t *)msg, strlen(msg), MQTT_QOS_LEVEL_0);
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}