#include <sys/ctimer.h>
#include <sys/timer.h>
#include <net/linkaddr.h>
#include <net/netstack.h>
#include <net/ipv6/uip.h>
#include <net/ipv6/uip-ds6.h>
#include <net/ipv6/uip-ds6-nbr.h>
#include <net/ipv6/uip-ds6-route.h>
#include <net/ipv6/uip-icmp6.h>
#include <net/ipv6/uip-nameserver.h>
#include <net/ipv6/uiplib.h>
#include <net/ipv6/udp-socket.h>
#include <net/ipv6/uip-udp-packet.h>
#include <net/mac/framer/framer.h>
#include <net/routing/routing.h>
#include <contiki.h>
#include <contiki-conf.h>
#include <contiki-net.h>
#include <contiki-lib.h>
#include <string.h>
#include <mqtt.h>
#include <dht11-sensor.h>
#include <dev/rgb-led/rgb-led.h>
#include "coap-engine.h"
#include "coap-log.h"

static struct mqtt_connection conn;
static struct mqtt_message *msg;
static char *host = "47.102.103.1";
static uint16_t port = 1883;
static char *client_id = "nrf52840-client";
static char *pub_topic = "device/state";
static clock_time_t publish_interval = CLOCK_SECOND * 5;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t ev, void *data) {
    if(ev == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if(ev == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static struct etimer periodic_timer;
    static uint16_t msgid;
    
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publish_process, client_id, mqtt_event_callback, MQTT_CLIENT_MAX_SEGMENT_SIZE);
    mqtt_connect(&conn, host, port, MQTT_CLEAN_SESSION_ON);

    etimer_set(&periodic_timer, publish_interval);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&periodic_timer)) {
            char msg_buf[20];
            strcpy(msg_buf, "work or not work");
            mqtt_publish(&conn, &msgid, pub_topic, (uint8_t *)msg_buf, strlen(msg_buf), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}