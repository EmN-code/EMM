#include "net/mac/framer/framer.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "net/linkaddr.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "mqtt.h"
#include "random.h"
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "device/state"
#define MQTT_STATE_WORK "work"
#define MQTT_STATE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

PROCESS(mqtt_pub_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_pub_process);

struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS_THREAD(mqtt_pub_process, ev, data)
{
  static char client_id[32];
  static char pub_message[20];

  PROCESS_BEGIN();

  snprintf(client_id, sizeof(client_id), MQTT_CLIENT_ID);
  mqtt_register(&conn, &mqtt_pub_process, client_id, NULL, MAX_TCP_SEGMENT_SIZE);

  mqtt_set_username_password(&conn, "", "");

  mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60);

  etimer_set(&publish_timer, PUBLISH_INTERVAL);
  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&publish_timer)) {
      snprintf(pub_message, sizeof(pub_message), MQTT_STATE_WORK);
      mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

      etimer_reset(&publish_timer);
    }
  }

  PROCESS_END();
}