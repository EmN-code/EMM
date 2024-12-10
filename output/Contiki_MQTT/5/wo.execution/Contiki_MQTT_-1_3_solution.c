#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "dht11-sensor.h"
#include "coap.h"
#include "net/ipv6/uip-sr.h"
#include "net/mac/framer/framer.h"
#include "net/netstack.h"
#include "contiki-default-conf.h"
#include "contiki-net.h"
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "dev/button-hal.h"

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data)
{
    PROCESS_BEGIN();

    const char *mqtt_broker_ip = "47.102.103.1";
    uint16_t mqtt_broker_port = 1883;
    static char *pub_topic = "device/state";
    static char *pub_message = "work";
   
    etimer_set(&periodic_timer, 5 * CLOCK_SECOND);

    mqtt_register(&conn, &mqtt_example_process, "test_client", NULL, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, mqtt_broker_ip, mqtt_broker_port, KEEP_ALIVE_TIMER);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}