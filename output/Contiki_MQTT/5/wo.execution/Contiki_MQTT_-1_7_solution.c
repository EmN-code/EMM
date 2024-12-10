#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-icmp6.h"
#include "sys/timer.h"
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "sys/ctimer.h"
#include "random.h"
#include "dht11-sensor.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "coap.h"
#include "coap-blocking-api.h"
#include "coap-engine.h"
#include "coap-log.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "net/netstack.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dev/button-hal.h"
#include "contiki.h"
#include "net/mac/mac.h"
#include "net/mac/framer/framer.h"
#include "net/linkaddr.h"
#include "net/routing/routing.h"
#include "string.h"
#include "stdio.h"

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define KEEP_ALIVE 60
#define MQTT_PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_TOPIC_STATE "device/state"
#define CLIENT_ID "nrf52840_device"
#define MAX_TCP_SEGMENT_SIZE 32

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char client_id[] = CLIENT_ID;
static char mqtt_broker_address[] = BROKER_IP_ADDR;

PROCESS(mqtt_example_process, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example_process);

static int state = 0; // 0: not work, 1: work

static void publish_state(void) {
    char payload[20];
    uint16_t mid;
    snprintf(payload, sizeof(payload), "State: %s", state ? "work" : "not work");
    mqtt_publish(&conn, &mid, MQTT_TOPIC_STATE, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
}

PROCESS_THREAD(mqtt_example_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_status_t status;
    char username[] = "user";
    char password[] = "pass";

    mqtt_register(&conn, &mqtt_example_process, client_id, NULL, MAX_TCP_SEGMENT_SIZE);
    mqtt_set_username_password(&conn, username, password);

    status = mqtt_connect(&conn, mqtt_broker_address, BROKER_PORT, KEEP_ALIVE, MQTT_CLEAN_SESSION_ON);

    if(status != MQTT_STATUS_OK) {
        printf("Error connecting to MQTT broker: %u\n", status);
        PROCESS_EXIT();
    }

    etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&publish_timer)) {
            publish_state();
            etimer_reset(&publish_timer);
            state = !state; // Toggle state
        }
    }

    PROCESS_END();
}