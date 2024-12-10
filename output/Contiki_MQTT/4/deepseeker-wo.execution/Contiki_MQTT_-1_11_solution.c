
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/mac/framer/framer.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/psock.h"
#include "net/mac/mac.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "sys/rtimer.h"
#include "sys/log.h"
#include "string.h"
#include "random.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct etimer publish_timer;
static struct mqtt_connection mqtt_conn;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    LOG_INFO("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    LOG_INFO("MQTT disconnected\n");
  }
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  mqtt_register(&mqtt_conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_callback, 128);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

    if (mqtt_connected(&mqtt_conn)) {
      mqtt_publish(&mqtt_conn, NULL, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, MQTT_PAYLOAD_SIZE, MQTT_QOS_0, 0, 0, NULL);
    }

    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}
