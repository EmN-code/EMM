#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "sys/timer.h"
#include "sys/clock.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dev/button-hal.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "random.h"
#include "string.h"
#include "dht11-sensor.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_PUB_INTERVAL 5
#define PUBLISH_TOPIC "device/state"
#define CLIENT_ID "nrf52840"

static struct mqtt_connection conn;
static char pub_message[20];
static struct etimer periodic_timer;

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *ptr) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

void publish_state() {
    snprintf(pub_message, sizeof(pub_message), "work"); // Assume it always works
    mqtt_publish(&conn, NULL, PUBLISH_TOPIC, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_NOT_RETAIN);
}

PROCESS(mqtt_pub_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_pub_process);

PROCESS_THREAD(mqtt_pub_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_pub_process, CLIENT_ID, mqtt_event_callback, 256);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60, MQTT_CLEAN_SESSION_ON);

    etimer_set(&periodic_timer, CLOCK_SECOND * MQTT_PUB_INTERVAL);
    while(1) {
        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER) {
            publish_state();
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}