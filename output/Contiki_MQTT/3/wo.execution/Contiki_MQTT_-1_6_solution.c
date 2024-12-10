#include <string.h>
#include "coap-engine.h"
#include "coap-log.h"
#include "contiki.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dht11-sensor.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip.h"
#include "net/linkaddr.h"
#include "net/mac/framer/framer.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "random.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "sys/timer.h"
#include "udp-socket.h"

#define LOG_MODULE "MQTT-Example"
#define LOG_LEVEL LOG_LEVEL_DBG

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840_mqtt_client"
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_TOPIC "nrf52840/status"

static struct mqtt_connection conn;
static struct etimer periodic_timer;
static char broker_address[CONFIG_IP_ADDR_STR_MAX_LEN];

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void mqtt_event_cb(struct mqtt_connection *m, mqtt_event_t event, void *data)
{
  switch (event) {
    case MQTT_EVENT_CONNECTED:
      LOG_INFO("Connected to MQTT broker.\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      LOG_ERR("Disconnected from MQTT broker.\n");
      break;
    case MQTT_EVENT_PUBLISHED:
      LOG_INFO("Publishing complete.\n");
      break;
    case MQTT_EVENT_SUBACK:
    case MQTT_EVENT_UNSUBACK:
    case MQTT_EVENT_PUBACK:
      break;
    case MQTT_EVENT_ERROR:
      LOG_ERR("MQTT Error occurred: %u\n", *((mqtt_event_error_t *)data));
      break;
    default:
      LOG_ERR("MQTT event not handled.\n");
      break;
  }
}

PROCESS_THREAD(mqtt_example_process, ev, data)
{
  PROCESS_BEGIN();
  
  LOG_INFO("Starting MQTT example\n");
  mqtt_register(&conn, &mqtt_example_process, CLIENT_ID, mqtt_event_cb, MQTT_MAX_SEGMENT_SIZE);

  snprintf(broker_address, sizeof(broker_address), "tcp://%s:%u", BROKER_IP, BROKER_PORT);

  mqtt_set_username_password(&conn, "username", "password");

  etimer_set(&periodic_timer, PUBLISH_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &periodic_timer);

    char *state_message = "work";
    mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)state_message, strlen(state_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

    LOG_INFO("Published: %s\n", state_message);

    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}