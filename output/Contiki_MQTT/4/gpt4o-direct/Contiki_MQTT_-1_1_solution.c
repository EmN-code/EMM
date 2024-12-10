#include "contiki.h"
#include "contiki-net.h"
#include "mqtt.h"
#include "log.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840_client"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define MQTT_TOPIC "device/state"

static struct etimer publish_timer;
static struct mqtt_connection conn;
static char state_msg[20];

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void
mqtt_event(struct mqtt_connection *m, mqtt_event_t event,
           void *data)
{
  switch (event) {
    case MQTT_EVENT_CONNECTED:
      LOG_INFO("MQTT connected\n");
      etimer_set(&publish_timer, PUBLISH_INTERVAL);
      break;
    case MQTT_EVENT_DISCONNECTED:
      LOG_WARN("MQTT disconnected\n");
      break;
    case MQTT_EVENT_PUBLISH_CONFIRMED:
      LOG_INFO("Publish confirmed\n");
      break;
    default:
      LOG_WARN("MQTT event %d\n", event);
      break;
  }
}

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_publish_process, CLIENT_ID, mqtt_event,
                MAX_TCP_SEGMENT_SIZE);

  uip_ipaddr_t broker_ipaddr;
  uip_ip6addr(&broker_ipaddr, 0x2b67, 0x6701, 0, 0, 0, 0, 0, 0x1);
  mqtt_connect(&conn, &broker_ipaddr, BROKER_PORT,
               (20 * CLOCK_SECOND), MQTT_CLEAN_SESSION_ON);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
    snprintf(state_msg, sizeof(state_msg), "work or not work");
    mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)state_msg,
                 strlen(state_msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
    LOG_INFO("Published state: %s\n", state_msg);
    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}