
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include "sys/etimer.h"
#include "string.h"
#include "sys/clock.h"

#define LOG_MODULE "MQTT Example"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_KEEP_ALIVE 60

static struct simple_udp_connection udp_conn;
static struct etimer et;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen)
{
  LOG_INFO("Received response from broker\n");
}

PROCESS_THREAD(mqtt_example_process, ev, data)
{
  static struct mqtt_connection conn;
  static uint16_t mid;
  static char payload[] = MQTT_PAYLOAD;

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, 0, NULL, MQTT_BROKER_PORT, udp_rx_callback);

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    LOG_INFO("Connecting to MQTT broker...\n");
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE);

    LOG_INFO("Publishing message...\n");
    mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), 0);

    LOG_INFO("Disconnecting from MQTT broker...\n");
    mqtt_disconnect(&conn);
  }

  PROCESS_END();
}
