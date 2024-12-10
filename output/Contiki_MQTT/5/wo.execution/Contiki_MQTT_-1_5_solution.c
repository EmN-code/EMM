#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "sys/rtimer.h"
#include "sys/timer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "contiki-default-conf.h"
#include "mqtt-prop.h"
#include "rgb-led.h"
#include "random.h"
#include "net/netstack.h"
#include "dht11-sensor.h"
#include "dev/button-hal.h"
#include "coap-log.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "mqtt.h"
#include "net/linkaddr.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "net/routing/routing.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/routing/routing.h"
#include "dev/button-hal.h"

static struct etimer periodic_timer;
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define BUFFER_SIZE 64

static char client_id[32];
static char pub_topic[BUFFER_SIZE];
static char message[BUFFER_SIZE];

static struct mqtt_connection connection;
static struct mqtt_message message;

PROCESS(mqtt_client_example, "MQTT Client Example");
AUTOSTART_PROCESSES(&mqtt_client_example);

static void
mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *event_data)
{
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}

PROCESS_THREAD(mqtt_client_example, ev, data)
{
    PROCESS_BEGIN();

    snprintf(client_id, sizeof(client_id), "nrf52840_device");
    mqtt_register(&connection, &mqtt_client_example, client_id, mqtt_event_callback, BUFFER_SIZE);

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        etimer_reset(&periodic_timer);

        mqtt_connect(&connection, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL / CLOCK_SECOND);

        snprintf(pub_topic, sizeof(pub_topic), "device/status");
        snprintf(message, sizeof(message), "state: %s", (random_rand() % 2 == 0) ? "work" : "not work");

        mqtt_publish(&connection, NULL, pub_topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
    }

    PROCESS_END();
}