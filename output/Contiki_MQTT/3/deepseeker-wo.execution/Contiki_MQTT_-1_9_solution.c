
#include "contiki-default-conf.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "coap-blocking-api.h"
#include "random.h"
#include "sys/process.h"
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
#include "mqtt-prop.h"
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
#include "mqtt.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki-lib.h"
#include "sys/timer.h"
#include "sys/process.h"
#include "coap-engine.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "contiki.h"
#include "sys/log.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-nameserver.h"
#include "contiki.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "stdbool.h"
#include "sys/clock.h"
#include "coap-log.h"
#include "sys/log.h"
#include "dht11-sensor.h"
#include "contiki-net.h"
#include "net/ipv6/psock.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_KEEP_ALIVE 60

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct mqtt_connection mqtt_conn;
static uint8_t mqtt_payload[] = MQTT_PAYLOAD;
static uint16_t mqtt_payload_len = sizeof(mqTT_PAYLOAD) - 1;

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    static struct etimer et;
    static uint16_t mid;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE);
        mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, mqtt_payload, mqtt_payload_len, MQTT_QOS);
        mqtt_disconnect(&mqtt_conn);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void)
{
    process_start(&mqtt_client_process, NULL);
    return 0;
}
