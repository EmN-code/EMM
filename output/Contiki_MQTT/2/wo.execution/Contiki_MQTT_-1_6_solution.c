#include "string.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "dht11-sensor.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static char client_id[32];
static struct ctimer update_timer;

static void publish_state(void *data) {
  static char state_msg[20];
  static uint16_t msg_id;

  snprintf(state_msg, sizeof(state_msg), "State: %s", "work");
  mqtt_publish(&conn, &msg_id, "state_topic", (uint8_t *)state_msg,
               strlen(state_msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
  
  ctimer_reset(&update_timer);
}

static void mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event,
                               void *data) {
  if (event == MQTT_EVENT_CONNECTED) {
    printf("Connected to MQTT broker\n");
  } else if (event == MQTT_EVENT_DISCONNECTED) {
    printf("Disconnected from MQTT broker\n");
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL, 1);  // Reconnect
  }
}

PROCESS(mqtt_client_process, "MQTT Client");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data) {
  PROCESS_BEGIN();

  snprintf(client_id, sizeof(client_id), "%02x%02x%02x%02x%02x%02x", 
           linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1],
           linkaddr_node_addr.u8[2], linkaddr_node_addr.u8[3],
           linkaddr_node_addr.u8[4], linkaddr_node_addr.u8[5]);

  mqtt_register(&conn, &mqtt_client_process, client_id, mqtt_event_handler, 256);

  printf("Connecting to MQTT broker\n");
  mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL, 1, NULL);

  ctimer_set(&update_timer, PUBLISH_INTERVAL, publish_state, NULL);

  PROCESS_YIELD();

  PROCESS_END();
}