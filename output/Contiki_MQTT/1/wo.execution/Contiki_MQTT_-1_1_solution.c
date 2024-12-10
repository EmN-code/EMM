#include "simple-udp.h"
#include "net/ipv6/uip.h"
#include "mqtt.h"
#include "etimer.h"
#include "stdio.h"
#include "contiki.h"

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static struct mqtt_connection conn;
static struct etimer et;

static void
mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data)
{
  switch(event) {
  case MQTT_EVENT_CONNECTED:
    printf("MQTT connection established\n");
    break;
  case MQTT_EVENT_DISCONNECTED:
    printf("MQTT disconnected\n");
    break;
  default:
    break;
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_publish_process, "client_id", mqtt_event, 256);

  mqtt_connect(&conn, "47.102.103.1", 1883, 1000);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    const char *topic = "device/state";
    const char *message = "work";

    mqtt_publish(&conn, NULL, topic, (uint8_t *)message, strlen(message) + 1, MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

    printf("Published: %s\n", message);

    etimer_reset(&et);
  }

  PROCESS_END();
}