#include <stdio.h>
#include "mqtt.h"  // Assuming header provides necessary MQTT functions and types
#include "contiki.h"
#include "net/ipv6/tcp-socket.h"

PROCESS(mqtt_example_process, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static struct etimer timer;

PROCESS_THREAD(mqtt_example_process, ev, data)
{
  PROCESS_BEGIN();

  static char client_id[] = "nrf52840";
  static char pub_topic[] = "state";
  static char pub_payload[] = "work or not work";
  static uint16_t msg_id;
  static mqtt_status_t status;

  // Register and connect to the broker
  status = mqtt_register(&conn, &mqtt_example_process, client_id, MQTT_ROLLBACK_ON_ERROR, 100);
  if (status != MQTT_STATUS_OK) {
    printf("Failed to register MQTT connection\n");
    PROCESS_EXIT();
  }

  status = mqtt_connect(&conn, "47.102.103.1", 1883, MQTT_CLEAN_SESSION_ON);
  if (status != MQTT_STATUS_OK) {
    printf("Failed to connect to MQTT broker\n");
    PROCESS_EXIT();
  }

  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    mqtt_publish(&conn, &msg_id, pub_topic, (uint8_t *)pub_payload, sizeof(pub_payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  process_start(&mqtt_example_process, NULL);
  while(1) {
    process_run();
    etimer_request_poll();
  }
}