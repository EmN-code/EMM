#include "net/ipv6/uiplib.h"
#include "net/ipv6/udp-socket.h"
#include "contiki.h"
#include "mqtt.h"
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_CLIENT_ID "nrf52840-client"
#define MQTT_TOPIC "state"

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(nrf52840_mqtt_process, "NRF52840 MQTT Publish Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_process);

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch(event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to the MQTT broker.\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from the MQTT broker.\n");
      break;
    default:
      printf("An event occurred: %d\n", event);
      break;
  }
}

PROCESS_THREAD(nrf52840_mqtt_process, ev, data) {
  PROCESS_BEGIN();

  char client_id[] = MQTT_CLIENT_ID;
  mqtt_register(&conn, &nrf52840_mqtt_process, client_id, mqtt_event_callback, 100);

  etimer_set(&periodic_timer, PUBLISH_INTERVAL);

  while (1) {
    PROCESS_YIELD();
    if (ev == PROCESS_EVENT_TIMER && data == &periodic_timer) {
      static char payload[] = "work or not work";
      uint16_t mid;
      mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, sizeof(payload) - 1, MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_OFF, NULL);
      printf("Published state to topic.\n");
      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&nrf52840_mqtt_process, NULL);
  while(1) {
    process_run();
  }
  return 0;
}