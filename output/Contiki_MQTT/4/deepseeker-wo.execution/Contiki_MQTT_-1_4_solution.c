
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_TOPIC_ALIAS 0

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct simple_udp_connection udp_conn;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    static struct etimer et;
    static struct mqtt_connection conn;
    static uint16_t mid;
    static char payload[] = MQTT_PAYLOAD;
    static char topic[] = MQTT_TOPIC;

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, 0, NULL, 0, NULL);

    mqtt_register(&conn, PROCESS_CURRENT(), "client_id", mqtt_event_callback, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        mqtt_publish(&conn, &mid, topic, (uint8_t *)payload, strlen(payload), MQTT_QOS, MQTT_RETAIN, MQTT_TOPIC_ALIAS, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&mqtt_client_process, NULL);
    return 0;
}
