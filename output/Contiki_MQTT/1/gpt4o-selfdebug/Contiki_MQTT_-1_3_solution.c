#include "net/netstack.h"
#include "mqtt.h"
#include "stdio.h"
#include "stdint.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct etimer publish_timer;
static struct mqtt_connection conn;
static char *client_id = "contiki_nrf52840";

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
  PROCESS_BEGIN();
  mqtt_status_t status;
  static uint16_t msg_id;

  // Set timer for publishing
  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  // Set up the MQTT client
  mqtt_register(&conn, &mqtt_publisher_process, client_id, mqtt_event, 256);
  
  // Connect to the MQTT broker
  status = mqtt_connect(&conn, BROKER_IP, BROKER_PORT, (PUBLISH_INTERVAL * 3) / CLOCK_SECOND, MQTT_CLEAN_SESSION_ON);
  if(status != MQTT_STATUS_OK) {
    printf("Failed to connect to MQTT broker\n");
    PROCESS_EXIT();
  }

  while(1) {
    PROCESS_YIELD();
    if(ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
      // Publish the state message
      const char *pub_topic = "device/state";
      const char *pub_payload = "work";
      status = mqtt_publish(&conn, &msg_id, pub_topic, (uint8_t *)pub_payload, strlen(pub_payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, 0);

      if(status == MQTT_STATUS_OK) {
        printf("Message published successfully\n");
      } else {
        printf("Failed to publish message\n");
      }

      // Reset timer for next publish
      etimer_reset(&publish_timer);
    }
  }

  PROCESS_END();
}

void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch(event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to MQTT broker\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from MQTT broker\n");
      break;
    default:
      printf("MQTT event: %d\n", event);
      break;
  }
}