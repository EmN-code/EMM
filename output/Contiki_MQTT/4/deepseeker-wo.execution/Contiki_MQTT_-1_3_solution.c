
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "dev/button-hal.h"
#include "random.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/routing/routing.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "sys/rtimer.h"
#include "dht11-sensor.h"
#include "coap-engine.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/psock.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_KEEP_ALIVE 60

static struct simple_udp_connection udp_conn;
static struct etimer et;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

void publish_state(void) {
    static uint16_t mid = 0;
    mqtt_status_t status = mqtt_publish(NULL, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, sizeof(MQTT_PAYLOAD) - 1, MQTT_QOS_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("MQTT publish failed\n");
    }
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();

    static struct mqtt_connection conn;
    static char client_id[] = "nrf52840_client";

    mqtt_register(&conn, PROCESS_CURRENT(), client_id, mqtt_event_callback, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        etimer_reset(&et);
        publish_state();
    }

    PROCESS_END();
}
