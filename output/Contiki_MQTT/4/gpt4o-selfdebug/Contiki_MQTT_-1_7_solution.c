// Description: The following code is a Contiki-NG application for the nRF52840 platform. It sets up an MQTT client that connects to a broker at 47.102.103.1 on port 1883. The client publishes its state ("work or not work") every five seconds.

#include <string.h>
#include <stdio.h>
#include "sys/etimer.h"
#include "mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840-client"
#define TOPIC "state"

// Function prototypes
void mqtt_event_callback(mqtt_connection_t *m, mqtt_event_t event, void *data);

PROCESS(mqtt_client_process, "MQTT Client Example");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
  static struct etimer timer;
  static mqtt_connection_t conn;
  static char state_message[20];

  PROCESS_BEGIN();

  // Initialize MQTT connection
  mqtt_register(&conn, &mqtt_client_process, CLIENT_ID, mqtt_event_callback, MQTT_MAX_SEGMENT_SIZE);
  mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 1000);

  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_YIELD();

    if (ev == PROCESS_EVENT_TIMER) {
      strcpy(state_message, "work"); // Example state
      uint16_t msg_id;
      mqtt_publish(&conn, &msg_id, TOPIC, (uint8_t *)state_message, strlen(state_message), MQTT_QOS_LEVEL_0, MQTT_ARRANGE_NOT_RETAINED, MQTT_TOPIC_ALIAS_DISABLED, NULL);

      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}

void mqtt_event_callback(mqtt_connection_t *m, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT client connected to broker\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT client disconnected from broker\n");
  }
}
