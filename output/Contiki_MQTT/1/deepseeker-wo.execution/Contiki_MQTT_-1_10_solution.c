
#include "contiki-default-conf.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "coap-blocking-api.h"
#include "lib/random.h"
#include "string.h"
#include "net/ipv6/tcp-socket.h"
#include "dev/leds.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcpip.h"
#include "contiki-conf.h"
#include "net/ipv6/simple-udp.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/udp-socket.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "sys/rtimer.h"
#include "contiki-lib.h"
#include "sys/timer.h"
#include "coap-engine.h"
#include "sys/log.h"
#include "sys/etimer.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/netstack.h"
#include "net/ipv6/psock.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

#define MQTT_QOS_0 0
#define MQTT_RETAIN_FALSE 0
#define MQTT_TOPIC_ALIAS_EN_FALSE 0
#define MQTT_STATUS_SUCCESS 0

#define LOG_LEVEL LOG_LEVEL_INFO
#define LOG_MODULE "MQTT"

struct mqtt_connection {
  int dummy; // Placeholder to avoid storage size error
};

typedef void (*mqtt_event_callback_t)(void);
typedef int mqtt_status_t;

mqtt_status_t mqtt_publish(struct mqtt_connection *conn,
                           uint16_t *mid,
                           char *topic,
                           uint8_t *payload,
                           uint32_t payload_size,
                           int qos_level,
                           int retain,
                           int topic_alias_en,
                           void *prop_list) {
  // Placeholder implementation
  return MQTT_STATUS_SUCCESS;
}

mqtt_status_t mqtt_connect(struct mqtt_connection *conn,
                           char *host,
                           uint16_t port,
                           uint16_t keep_alive,
                           int clean_session,
                           void *prop_list) {
  // Placeholder implementation
  return MQTT_STATUS_SUCCESS;
}

void mqtt_register(struct mqtt_connection *conn,
                   void *app_process,
                   char *client_id,
                   mqtt_event_callback_t event_callback,
                   uint16_t max_segment_size) {
  // Placeholder implementation
}

static struct mqtt_connection conn;
static struct etimer et;

static void mqtt_event_handler(void) {
  // Placeholder for event handling
}

int main(void) {
  mqtt_register(&conn, NULL, MQTT_CLIENT_ID, mqtt_event_handler, 0);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_YIELD();
    if(etimer_expired(&et)) {
      etimer_reset(&et);
      uint8_t payload[] = "work";
      mqtt_status_t status = mqtt_publish(&conn, NULL, MQTT_TOPIC, payload, sizeof(payload) - 1, MQTT_QOS_0, MQTT_RETAIN_FALSE, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
      if(status == MQTT_STATUS_SUCCESS) {
        LOG_INFO("Published state: work\n");
      } else {
        LOG_INFO("Failed to publish state\n");
      }
    }
  }

  return 0;
}
