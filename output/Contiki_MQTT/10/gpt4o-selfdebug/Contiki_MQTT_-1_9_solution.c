#include <string.h>
#include <stdio.h>
#include <mqtt.h>
#include <net/ipv6/tcp-socket.h>
#include <net/ipv6/tcpip.h>
#include <contiki.h>

PROCESS(mqtt_client_process, "MQTT Client");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct mqtt_connection conn;
static char *broker_ip = "47.102.103.1";
static uint16_t broker_port = 1883;

static void init_mqtt(void) {
    mqtt_register(&conn, &mqtt_client_process, "nrf52840", NULL, MQTT_MAX_SEGMENT_SIZE);
    mqtt_connect(&conn, broker_ip, broker_port, 60, MQTT_CLEAN_SESSION);
}

static void publish_state(void) {
    uint16_t mid;
    char *topic = "nrf52840/status";
    char *message = "work";
    mqtt_publish(&conn, &mid, topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_DISABLED, NULL);
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();
    init_mqtt();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_YIELD();
        if (etimer_expired(&timer)) {
            publish_state();
            etimer_reset(&timer);
        }
    }
    PROCESS_END();
}