
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/uart1.h"
#include "mqtt.h"
#include <stdio.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_connection mqtt_conn;
static struct etimer et;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void
mqtt_connect(void)
{
  mqtt_conn.broker_ip = MQTT_BROKER_IP;
  mqtt_conn.broker_port = MQTT_BROKER_PORT;
  mqtt_conn.client_id = "nrf52840_client";
  mqtt_conn.username = NULL;
  mqtt_conn.password = NULL;

  mqtt_status_t status = mqtt_connect(&mqtt_conn);
  if (status != MQTT_STATUS_OK) {
    printf("MQTT connection failed\n");
  } else {
    printf("MQTT connected\n");
  }
}

static void
mqtt_publish_state(void)
{
  uint16_t mid;
  mqtt_status_t status = mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS_LEVEL_0, MQTT_RETAIN_FALSE, 0, MQTT_TOPIC_ALIAS_EN_FALSE, NULL);
  if (status != MQTT_STATUS_OK) {
    printf("MQTT publish failed\n");
  } else {
    printf("MQTT published: %s\n", MQTT_PAYLOAD);
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
  PROCESS_BEGIN();

  mqtt_connect();

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    mqtt_publish_state();

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&mqtt_publish_process, NULL);
  return 0;
}
