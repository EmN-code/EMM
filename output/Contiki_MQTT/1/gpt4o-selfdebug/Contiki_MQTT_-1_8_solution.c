// Include necessary headers in descending order
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/process.h"
#include "mqtt/mqtt.h"
#include "stdio.h"

// Define the process
PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

// Define MQTT broker details
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

// Define publish interval
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

// Define states
static char* state_message = "work";
static struct etimer publish_timer;

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  static struct mqtt_connection conn;
  PROCESS_BEGIN();

  // Setting the timer to publish every 5 seconds
  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  // Configure the MQTT connection
  mqtt_register(&conn, &mqtt_publish_process, "mqtt_client", mqtt_event, 512);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, PUBLISH_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
    
    uint16_t msg_id;
    mqtt_publish(&conn, &msg_id, "state_topic", (uint8_t *)state_message, strlen(state_message), MQTT_QOS_LEVEL_0);
    
    // Reset the timer
    etimer_reset(&publish_timer);
    }

  PROCESS_END();
}