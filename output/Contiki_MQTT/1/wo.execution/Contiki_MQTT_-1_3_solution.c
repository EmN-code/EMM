#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/tcp-socket.h"
#include "dht11-sensor.h"
#include "net/netstack.h"
#include "net/ipv6/uip-ds6.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-icmp6.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/button-hal.h"
#include "mqtt-prop.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-ds6-route.h"
#include "mqtt.h"

#define MQTT_BROKER_IP     "47.102.103.1"
#define MQTT_BROKER_PORT   1883
#define CLIENT_ID          "nrf52840_client"
#define PUBLISH_INTERVAL   5 
#define MQTT_TOPIC         "device/state"

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_client_process, CLIENT_ID, mqtt_event_callback, 128);

    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60 * CLOCK_SECOND, MQTT_CLEAN_SESSION_ON);

    etimer_set(&periodic_timer, PUBLISH_INTERVAL * CLOCK_SECOND);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        static const char payload[] = "work";

        mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)payload, sizeof(payload) - 1, MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("MQTT client connected\n");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT client disconnected\n");
    }
}