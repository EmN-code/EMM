
#include "contiki.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "string.h"
#include "stdio.h"
#include "random.h"
#include "sys/process.h"
#include "sys/clock.h"
#include "sys/rtimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "dev/button-hal.h"
#include "dev/rgb-led/rgb-led.h"
#include "coap-timer.h"
#include "coap-observe-client.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "coap-engine.h"
#include "coap-request-state.h"
#include "coap-observe.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/psock.h"
#include "dht11-sensor.h"

#define LOG_MODULE "MQTT Example"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

static struct simple_udp_connection udp_conn;
static struct etimer et;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void
mqtt_event_handler(struct mqtt_connection *c, mqtt_event_t event, void *data)
{
  if(event == MQTT_EVENT_CONNECT) {
    LOG_INFO("MQTT connected\n");
  } else if(event == MQTT_EVENT_DISCONNECT) {
    LOG_INFO("MQTT disconnected\n");
  }
}

static void
publish_state(struct mqtt_connection *c)
{
  static const char *states[] = {"work", "not work"};
  const char *state = states[random_rand() % 2];
  mqtt_publish(c, MQTT_TOPIC, state, strlen(state), 0, 0);
  LOG_INFO("Published state: %s\n", state);
}

PROCESS_THREAD(mqtt_example_process, ev, data)
{
  static struct mqtt_connection mqtt_conn;
  static struct etimer et;

  PROCESS_BEGIN();

  LOG_INFO("Starting MQTT example\n");

  mqtt_init(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, mqtt_event_handler, NULL);
  mqtt_connect(&mqtt_conn);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_reset(&et);

    publish_state(&mqtt_conn);
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&mqtt_example_process, NULL);
  return 0;
}
