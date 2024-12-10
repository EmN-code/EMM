#include "sys/etimer.h"
#include "sys/clock.h"
#include "net/ipv6/uip.h"
#include "mqtt.h"

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_MAX_SEGMENT_SIZE 32
#define MQTT_KEEP_ALIVE_DEFAULT 60

static struct mqtt_connection conn;
static struct etimer publish_timer;

// MQTT Event Callback function
auto void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  if(event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if(event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

// Main function
int main() {
  // Initialize the MQTT connection
  mqtt_register(&conn, &mqtt_event);
  mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, MQTT_KEEP_ALIVE_DEFAULT, NULL);
  printf("Connecting to MQTT broker at %s:%d\n", BROKER_IP_ADDR, BROKER_PORT);

  // Main loop
  while(1) {
    etimer_set(&publish_timer, PUBLISH_INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

    // Publish message
    char *topic = "device/state";
    char *msg = "work";
    mqtt_publish(&conn, NULL, topic, (uint8_t *)msg, strlen(msg), MQTT_MAX_SEGMENT_SIZE);
    printf("Published message: %s\n", msg);
  }
  return 0;
}