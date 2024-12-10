#include <etimer.h>
#include <mqtt.h>
#include <mqtt_client.h>
#include <contiki.h>

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static struct etimer timer;
static struct mqtt_connection conn;
static char *broker_ip = "47.102.103.1";
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static void mqtt_event(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
  // Handle MQTT events here
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_publish_process, broker_ip, mqtt_event, PUBLISH_INTERVAL * CLOCK_SECOND);
  conn.auto_reconnect = 1;

  mqtt_connect(&conn, broker_ip, BROKER_PORT, PUBLISH_INTERVAL * CLOCK_SECOND, MQTT_CLEAN_SESSION_ON);

  etimer_set(&timer, CLOCK_SECOND * PUBLISH_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    const char *pub_topic = "state_topic";
    const char *pub_payload = "work";
    mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_payload, strlen(pub_payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
    etimer_reset(&timer);
  }

  PROCESS_END();
}