#include <net/ipv6/uiplib.h>
#include <net/ipv6/uip-udp-packet.h>
#include <net/ipv6/tcp-socket.h>

#include "contiki.h"
#include "mqtt.h"

PROCESS(nrf52840_mqtt_process, "NRF52840 MQTT Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_process);

static struct mqtt_connection conn;
static char *mqtt_topic = "device/state";
static char *mqtt_message = "work or not work";

static void publish_message(void) {
    uint16_t mid;
    uint8_t qos = 0; // QoS level 0
    mqtt_publish(&conn, &mid, mqtt_topic, (uint8_t *)mqtt_message, strlen(mqtt_message), qos);
}

PROCESS_THREAD(nrf52840_mqtt_process, ev, data) {
    static struct etimer timer;
    static char *host = "47.102.103.1";
    static uint16_t port = 1883;
    static uint16_t keep_alive = 60;
    static uint8_t clean_session = 1;

    PROCESS_BEGIN();

    mqtt_status_t status = mqtt_connect(&conn, host, port, keep_alive, clean_session, NULL);

    if (status == MQTT_STATUS_OK) {
        etimer_set(&timer, CLOCK_SECOND * 5);

        while (1) {
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
            publish_message();
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}