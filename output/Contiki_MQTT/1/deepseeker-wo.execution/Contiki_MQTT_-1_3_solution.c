
#include "contiki.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/simple-udp.h"
#include "contiki-conf.h"
#include "net/ipv6/uip-ds6-route.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-icmp6.h"
#include "contiki-default-conf.h"
#include "net/netstack.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "lib/random.h"
#include "dev/button-hal.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/routing/routing.h"
#include "net/ipv6/udp-socket.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "net/ipv6/tcp-socket.h"
#include "mqtt-prop.h"
#include "sys/ctimer.h"
#include "mqtt.h"
#include "contiki-lib.h"
#include "sys/timer.h"
#include "coap-log.h"
#include "sys/rtimer.h"
#include "dht11-sensor.h"
#include "net/ipv6/tcpip.h"
#include "coap-engine.h"
#include "net/ipv6/psock.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

static struct simple_udp_connection udp_conn;
static struct etimer et;

void mqtt_event_callback(struct mqtt_connection *c, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

void mqtt_topic_callback(struct mqtt_connection *c, const char *topic, const uint8_t *payload, uint16_t payload_len) {
  printf("Received message on topic %s: %.*s\n", topic, payload_len, payload);
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  static struct mqtt_connection mqtt_conn;
  static char message[64];
  static uint8_t ip_addr[16];

  PROCESS_BEGIN();

  // Initialize MQTT connection
  mqtt_conn.broker_ip = MQTT_BROKER_IP;
  mqtt_conn.broker_port = MQTT_BROKER_PORT;
  mqtt_conn.event_callback = mqtt_event_callback;
  mqtt_conn.topic_callback = mqtt_topic_callback;

  mqtt_connect(&mqtt_conn);

  // Set up UDP connection for MQTT
  simple_udp_register(&udp_conn, 0, NULL, 0, NULL);

  // Set up periodic timer
  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Reset timer
    etimer_reset(&et);

    // Publish message
    snprintf(message, sizeof(message), "%s", MQTT_MESSAGE);
    mqtt_publish(&mqtt_conn, MQTT_TOPIC, message, strlen(message));
  }

  PROCESS_END();
}
