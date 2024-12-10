#include "netstack.h"
#include "mqtt.h"
#include "simple-udp.h"

#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "device/state"
#define MQTT_CLIENT_ID "nrf52840_device"
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void mqtt_event(struct mqtt_connection *m, mqtt_event_callback_t event, void *data)
{
  // Handle MQTT events here
}

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_publish_process, MQTT_CLIENT_ID, mqtt_event, 0);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, PUBLISH_INTERVAL);

  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

    const char *msg = "work"; // or "not work" based on the actual state
    mqtt_publish(&conn, NULL, MQTT_PUBLISH_TOPIC, (uint8_t *)msg, strlen(msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}