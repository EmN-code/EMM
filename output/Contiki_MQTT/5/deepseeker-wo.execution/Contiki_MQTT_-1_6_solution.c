
#include "contiki-default-conf.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "coap-blocking-api.h"
#include "string.h"
#include "random.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6-route.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/ipv6/uip-icmp6.h"
#include "contiki-default-conf.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "contiki-net.h"
#include "mqtt-prop.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "contiki-conf.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "dev/gpio-hal.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-sr.h"
#include "string.h"
#include "net/routing/routing.h"
#include "sys/ctimer.h"
#include "mqtt.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki-lib.h"
#include "sys/timer.h"
#include "net/ipv6/uip-ds6-route.h"
#include "coap-engine.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "contiki.h"
#include "sys/log.h"
#include "stdio.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-nameserver.h"
#include "stdio.h"
#include "contiki.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "dev/gpio-hal.h"
#include "sys/clock.h"
#include "coap-log.h"
#include "dht11-sensor.h"
#include "contiki-net.h"
#include "net/ipv6/psock.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "state"
#define MQTT_QOS_LEVEL_0 0

static struct mqtt_connection conn;
static struct etimer et;

void event_callback(struct mqtt_connection *conn, mqtt_event_t event) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

int main(void) {
    PROCESS_BEGIN();

    // Initialize MQTT connection
    mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, event_callback, 128);
    mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, 1, NULL);

    // Set up periodic timer for publishing
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&et)) {
            etimer_reset(&et);

            // Publish state
            const char *state = (random_rand() % 2 == 0) ? "work" : "not work";
            mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, 0, 0, NULL);
        }
    }

    PROCESS_END();
}
