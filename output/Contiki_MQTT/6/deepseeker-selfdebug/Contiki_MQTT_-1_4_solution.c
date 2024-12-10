
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/tcpip.h"
#include "dev/uart0.h"  // Corrected to use uart0.h instead of uart1.h
#include "lib/random.h"
#include "sys/etimer.h"
#include "sys/process.h"
#include "sys/log.h"
#include "mqtt.h"

#define LOG_MODULE "MQTT App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

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
publish_state(void)
{
  mqtt_publish(&conn, NULL, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), 0, 0);
}

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
  static struct etimer connect_timer;

  PROCESS_BEGIN();

  tcpip_init(NULL, NULL);
  mqtt_init(&conn, NULL, NULL, 0, mqtt_event_handler, NULL);

  etimer_set(&connect_timer, CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&connect_timer));

  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 0, NULL);

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
