
#include "contiki.h"
#include "contiki-net.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "stdio.h"
#include "string.h"
#include "net/ipv6/simple-udp.h"
#include "dev/gpio-hal.h"

#define LOG_MODULE "MQTT Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct simple_udp_connection udp_conn;
static struct etimer et;

void udp_rx_callback(struct simple_udp_connection *c,
                     const uip_ipaddr_t *sender_addr,
                     uint16_t sender_port,
                     const uip_ipaddr_t *receiver_addr,
                     uint16_t receiver_port,
                     const uint8_t *data,
                     uint16_t datalen)
{
    LOG_INFO("Received response from broker\n");
}

void publish_state(const char *state)
{
    static char buffer[64];
    int len = snprintf(buffer, sizeof(buffer), "{\"state\":\"%s\"}", state);
    simple_udp_sendto(&udp_conn, buffer, len, uip_ipaddr(MQTT_BROKER_IP));
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    static struct etimer et;
    static char state[] = "work";

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, 0, NULL, MQTT_BROKER_PORT, udp_rx_callback);

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        publish_state(state);

        if(strcmp(state, "work") == 0) {
            strcpy(state, "not work");
        } else {
            strcpy(state, "work");
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}
