#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "contiki.h"
#include "mqtt.h"
#include "dht11-sensor.h"
#include "random.h"
#include "sys/etimer.h"
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define MQTT_CLIENT_ID "nrf52840-client"

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char topic[] = "nrf52840/state";
static char message[20];

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static void mqtt_event_cb(struct mqtt_connection *m, mqtt_event_t event, void *data) {
  switch (event) {
    case MQTT_EVENT_CONNECTED:
      printf("Connected to MQTT broker\n");
      break;
    case MQTT_EVENT_DISCONNECTED:
      printf("Disconnected from MQTT broker\n");
      break;
    case MQTT_EVENT_PUBLISH:
      printf("Message published\n");
      break;
    default:
      break;
  }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_publisher_process, MQTT_CLIENT_ID, mqtt_event_cb, MAX_TCP_SEGMENT_SIZE);

  conn.auto_reconnect = 0;

  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, (10*CLOCK_SECOND), 0);

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

    int random_value = random_rand() % 2; // Simulate work/not work state
    snprintf(message, sizeof(message), "State: %s", random_value ? "work" : "not work");
    mqtt_publish(&conn, NULL, topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0);

    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}