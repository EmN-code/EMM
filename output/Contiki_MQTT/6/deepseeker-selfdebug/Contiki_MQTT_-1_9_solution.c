
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "net/ip/tcpip.h"
#include "lib/random.h"
#include "dev/button-hal.h"
#include "dev/leds.h"
#include "sys/log.h"

#include <stdio.h>
#include <string.h>

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static struct etimer publish_timer;

static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
  LOG_INFO("Received response from broker\n");
}

static void
publish_state(void)
{
  static char state[] = "work";
  uip_ipaddr_t broker_ip;

  uip_ipaddr(&broker_ip, MQTT_BROKER_IP);

  if(simple_udp_sendto(&udp_conn, state, sizeof(state), &broker_ip)) {
    LOG_INFO("Published state: %s\n", state);
  } else {
    LOG_INFO("Failed to publish state\n");
  }
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, 0, NULL, MQTT_BROKER_PORT, udp_rx_callback);

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

    publish_state();

    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}
