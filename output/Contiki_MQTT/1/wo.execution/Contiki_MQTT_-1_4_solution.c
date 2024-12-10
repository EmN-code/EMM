#include "random.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcpip.h"
#include "net/mac/framer/framer.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "dht11-sensor.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dev/button-hal.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uiplib.h"

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static struct etimer periodic_timer;

static mqtt_status_t mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    // Handle different MQTT events here
    return MQTT_STATUS_OK;
}

PROCESS_THREAD(mqtt_example_process, ev, data){
    PROCESS_BEGIN();

    static char client_id[] = "nrf52840_client";
    static char pub_topic[] = "device/state";
    static char *pub_message;
    pub_message = "work";

    mqtt_register(&conn, &mqtt_example_process, client_id, mqtt_event_callback, 256);
    mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, 60 * CLOCK_SECOND, 0, 0);

    etimer_set(&periodic_timer, PUB_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}