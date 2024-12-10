
#include "contiki-default-conf.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/simple-udp.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/ipv6/uip-ds6-route.h"
#include "coap-timer.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "coap-observe.h"
#include "net/ipv6/udp-socket.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "coap-observe-client.h"
#include "contiki-conf.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-sr.h"
#include "string.h"
#include "net/routing/routing.h"
#include "sys/clock.h"
#include "sys/ctimer.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki-lib.h"
#include "sys/timer.h"
#include "sys/process.h"
#include "coap-engine.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "contiki.h"
#include "sys/log.h"
#include "net/ipv6/uip-nameserver.h"
#include "dev/button-hal.h"
#include "coap-log.h"
#include "dht11-sensor.h"
#include "net/ipv6/psock.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct mqtt_connection mqtt_conn;
static uint8_t mqtt_payload[] = MQTT_PAYLOAD;
static uint16_t mqtt_payload_len = sizeof(mqTT_PAYLOAD) - 1;

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    static struct etimer et;
    static uint16_t mid;

    PROCESS_BEGIN();

    mqtt_register(&mqtt_conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_callback, 128);
    mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (mqtt_connected(&mqtt_conn)) {
            mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, mqtt_payload, mqtt_payload_len, MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&mqtt_client_process, NULL);
    return 0;
}
