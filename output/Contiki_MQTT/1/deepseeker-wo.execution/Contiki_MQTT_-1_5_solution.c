
#include "contiki.h"
#include "sys/etimer.h"
#include "mqtt.h"
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection mqtt_conn;
static struct etimer et;
static char client_id[] = "nrf52840_client";

void mqtt_event_handler(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("MQTT connected\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("MQTT disconnected\n");
  }
}

void publish_state(struct mqtt_connection *conn) {
  static int state = 0;
  char *payload;

  if (state) {
    payload = MQTT_PAYLOAD_WORK;
    state = 0;
  } else {
    payload = MQTT_PAYLOAD_NOT_WORK;
    state = 1;
  }

  mqtt_publish(conn, NULL, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, 0, 0, NULL);
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&mqtt_conn, &mqtt_client_process, client_id, mqtt_event_handler, 128);
  mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

  etimer_set(&et, PUBLISH_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    if (mqtt_connected(&mqtt_conn)) {
      publish_state(&mqtt_conn);
    }
    etimer_reset(&et);
  }

  PROCESS_END();
}
