#include "contiki.h"
#include "mqtt.h"
#include "sys/etimer.h"

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct etimer timer;
static struct mqtt_connection conn;

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "device/state"
#define STATE "work"
#define OFF_STATE "not work"

static char client_id[64];

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  mqtt_register(&conn, &mqtt_publisher_process, client_id, NULL, 1000);
  mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, CLOCK_SECOND * 10, MQTT_CLEAN_SESSION_ON);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if(mqtt_connected(&conn)) {
      mqtt_publish(&conn, NULL, PUB_TOPIC, (uint8_t *)STATE, sizeof(STATE) - 1, MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}