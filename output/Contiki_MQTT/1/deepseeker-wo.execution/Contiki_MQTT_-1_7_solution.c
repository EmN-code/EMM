
#include "contiki-default-conf.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "coap-blocking-api.h"
#include "lib/random.h"
#include "string.h"
#include "net/ipv6/tcp-socket.h"
#include "dev/leds.h"
#include "net/ipv6/tcpip.h"
#include "contiki-conf.h"
#include "net/ipv6/simple-udp.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "contiki-lib.h"
#include "sys/timer.h"
#include "coap-engine.h"
#include "net/ipv6/uiplib.h"
#include "sys/log.h"
#include "net/ipv6/uip-nameserver.h"
#include "dev/button-hal.h"
#include "coap-log.h"
#include "dht11-sensor.h"
#include "net/ipv6/psock.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer publish_timer;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event) {
    if (event == MQTT_EVENT_CONNECTED) {
        LOG_INFO("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        LOG_INFO("MQTT disconnected\n");
    }
}

void publish_state(void) {
    static int state = 0;
    char message[20];
    mqtt_status_t status;

    snprintf(message, sizeof(message), "State: %s", state ? "work" : "not work");
    state = !state;

    status = mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_0, MQTT_RETAIN_FALSE, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
    if (status == MQTT_STATUS_SUCCESS) {
        LOG_INFO("Published: %s\n", message);
    } else {
        LOG_ERR("Failed to publish message\n");
    }
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    mqtt_register(&conn, PROCESS_CURRENT(), MQTT_CLIENT_ID, mqtt_event_callback, 0);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        publish_state();
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&mqtt_publisher_process, NULL);
    return 0;
}
