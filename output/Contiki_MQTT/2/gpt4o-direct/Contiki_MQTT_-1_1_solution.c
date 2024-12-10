#include "contiki.h"
#include "mqtt.h"
#include "net/ipv6/uip.h"
#include "sys/etimer.h"
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS(mqtt_example_process, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data) {
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_example_process, "mqtt_example", NULL, 0);
  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  static char pub_msg[20];

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

    if (!mqtt_connected(&conn)) {
      printf("Connecting to MQTT broker\n");
      uip_ip4addr_t ipaddr;
      uip_ip4addr(&ipaddr, 47, 102, 103, 1);
      mqtt_connect(&conn, &ipaddr, MQTT_BROKER_PORT, (60 * CLOCK_SECOND));
    }

    if (mqtt_connected(&conn)) {
      snprintf(pub_msg, sizeof(pub_msg), "state: %s", rand() % 2 == 0 ? "work" : "not work");
      printf("Publishing message: %s\n", pub_msg);
      mqtt_publish(&conn, NULL, "device/state", (uint8_t *)pub_msg, strlen(pub_msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
    } else {
      printf("Failed to connect to broker\n");
    }

    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}