#include <stdio.h>
#include "mqtt.h"
#include "contiki.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static mqtt_sn_con_t mqtt_conn;
static mqtt_status_t status;
static struct etimer periodic_timer;

PROCESS(example_mqtt_process, "MQTT Example_process");
AUTOSTART_PROCESSES(&example_mqtt_process);

PROCESS_THREAD(example_mqtt_process, ev, data)
{
  PROCESS_BEGIN();

  mqtt_register(&mqtt_conn, &example_mqtt_process, "client_id", mqtt_event_callback);
  status = mqtt_set_broker(&mqtt_conn, BROKER_IP, BROKER_PORT);

  if(status != MQTT_STATUS_OK) {
    printf("Failed to set broker: %d\n", status);
    PROCESS_EXIT();
  }

  while(1) {
    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    const char *message = "work";
    mqtt_publish(&mqtt_conn, NULL, "topic/state", (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

    printf("Published message: %s\n", message);
  }

  PROCESS_END();
}

static void mqtt_event_callback(mqtt_sn_con_t *m, mqtt_sn_event_t event, void *data)
{
  switch(event) {
    case MQTT_SN_EVENT_CONNECTED:
      printf("Connected to the MQTT broker\n");
      break;
    case MQTT_SN_EVENT_DISCONNECTED:
      printf("Disconnected from the MQTT broker\n");
      break;
    case MQTT_SN_EVENT_PUBLISHED:
      printf("Message published\n");
      break;
    default:
      printf("Unknown event\n");
      break;
  }
}