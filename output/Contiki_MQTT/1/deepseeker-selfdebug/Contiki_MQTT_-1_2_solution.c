
#include "contiki.h"
#include "dev/uart0.h"  // Ensure this header is included
#include "lib/sensors.h"
#include "lib/random.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "net/ip/mqtt.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection mqtt_conn;
static struct etimer publish_timer;

static void
mqtt_event_handler(struct mqtt_connection *conn, mqtt_event_t event, void *data)
{
  if(event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if(event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

static void
publish_state(void)
{
  static int state = 0;
  char *payload;

  if(state) {
    payload = MQTT_PAYLOAD_WORK;
  } else {
    payload = MQTT_PAYLOAD_NOT_WORK;
  }

  state = !state;

  mqtt_publish(&mqtt_conn, NULL, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
}

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
  PROCESS_BEGIN();

  uip_ipaddr_t broker_ip;
  uip_ipaddr(&broker_ip, 47, 102, 103, 1);

  mqtt_init(&mqtt_conn, mqtt_event_handler);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, MQTT_CLEAN_SESSION_TRUE, NULL);

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
    publish_state();
    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&mqtt_publish_process, NULL);
  return 0;
}
