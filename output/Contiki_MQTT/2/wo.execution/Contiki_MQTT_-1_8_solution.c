#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/netstack.h"
#include "sys/timer.h"
#include "mqtt.h"

#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_TOPIC "device/state"

PROCESS(nrf52840_mqtt_process, "NRF52840 MQTT Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_process);

static void publish_callback(struct mqtt_connection *m, const char *topic,
                             const uint8_t *chunk, uint16_t chunk_len) {
  // Callback for incoming messages
}

PROCESS_THREAD(nrf52840_mqtt_process, ev, data) {
  static struct etimer publish_timer;
  static struct mqtt_connection conn;
  static char client_id[] = MQTT_CLIENT_ID;
  static char pub_topic[] = MQTT_TOPIC;
  static char message[] = "work";

  PROCESS_BEGIN();

  mqtt_register(&conn, &nrf52840_mqtt_process, client_id, 
                publish_callback, MAX_TCP_SEGMENT_SIZE);
  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 
               PUBLISH_INTERVAL*3 /* keep alive */, 1 /* clean session */);

  etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

  while(1) {
    PROCESS_YIELD();

    if (ev == PROCESS_EVENT_TIMER && etimer_expired(&publish_timer)) {
      mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)message, 
                   strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

      etimer_reset(&publish_timer);
    }
  }

  PROCESS_END();
}