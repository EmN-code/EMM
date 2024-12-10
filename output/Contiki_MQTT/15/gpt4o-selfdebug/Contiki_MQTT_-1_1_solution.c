#include "netstack.h"
#include "mqtt.h"
#include "etimer.h"
#include "contiki.h"
#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS_THREAD(mqtt_example_process, ev, data)
{
  PROCESS_BEGIN();

  mqtt_register(&conn, &mqtt_example_process, "mqtt_example", NULL);
  etimer_set(&publish_timer, PUBLISH_INTERVAL);

  static char broker_address[64];
  snprintf(broker_address, sizeof(broker_address), "mqtt://%s:%d", MQTT_BROKER_IP, MQTT_BROKER_PORT);
  mqtt_connect(&conn, broker_address, 1883, 100, 0);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &publish_timer);
    printf("Publishing state...\n");
    static char *state = "work";
    mqtt_publish(&conn, NULL, "nrf52840/state", (uint8_t *)state, strlen(state), MQTT_QOS_LEVEL_0, 0);

    etimer_reset(&publish_timer);
  }

  PROCESS_END();
}

int main() {
    // The Contiki main process handles initialization
    printf("Starting Contiki MQTT example\n");
    process_start(&mqtt_example_process, NULL);
    return 0;
}