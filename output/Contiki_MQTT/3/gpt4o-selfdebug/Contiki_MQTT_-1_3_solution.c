#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-icmp6.h"
#include <stdio.h>
#include "contiki.h"
#include "mqtt.h"

#define BROKER_IPADDR  "47.102.103.1"
#define BROKER_PORT    1883
#define PUB_INTERVAL   (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publish_process, "nrf52840_publisher");
    conn.auto_reconnect = 0; // disable auto reconnect

    etimer_set(&periodic_timer, PUB_INTERVAL);
    mqtt_connect(&conn, BROKER_IPADDR, BROKER_PORT, PUB_INTERVAL * 3);

    while(1) {
        PROCESS_YIELD();
        if(etimer_expired(&periodic_timer)) {
            char *pub_topic = "device/state";
            char *pub_message = "work";
            mqtt_publish(&conn, NULL, pub_topic, pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, 0);

            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}

int main(void)
{
    process_start(&mqtt_publish_process, NULL);
    while (1) {
        process_run();
        etimer_request_poll();
    }
}