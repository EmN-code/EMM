
#include "contiki.h"
#include "dev/uart0.h"  // Ensure this header file is included
#include "lib/sensors.h"
#include "lib/random.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "net/ip/tcpip.h"
#include "net/ip/resolv.h"
#include "net/ip/uiplib.h"
#include "net/ip/psock.h"
#include "net/ip/tcp-socket.h"
#include "net/ip/mqtt.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/rtimer.h"
#include "sys/ctimer.h"
#include "sys/log.h"

#define LOG_MODULE "MQTT Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct etimer et;
static struct mqtt_connection mqtt_conn;
static char mqtt_buffer[128];

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static void
mqtt_publish(const char *topic, const char *payload)
{
  mqtt_publish_to_topic(&mqtt_conn, topic, payload, strlen(payload), 0, 0);
}

PROCESS_THREAD(mqtt_client_process, ev, data)
{
  static struct etimer et;
  static char state[16];

  PROCESS_BEGIN();

  LOG_INFO("Connecting to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

  mqtt_conn.ipaddr = uip_ipaddr_u32(uip_htonl(0x2F6666FF)); // Example IP address
  mqtt_conn.port = MQTT_BROKER_PORT;
  mqtt_conn.keepalive = 60;
  mqtt_conn.client_id = "nrf52840";
  mqtt_conn.username = NULL;
  mqtt_conn.password = NULL;
  mqtt_conn.will_topic = NULL;
  mqtt_conn.will_message = NULL;
  mqtt_conn.will_qos = 0;
  mqtt_conn.will_retain = 0;
  mqtt_conn.clean_session = 1;

  mqtt_connect(&mqtt_conn);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if(random_rand() % 2 == 0) {
      strcpy(state, "work");
    } else {
      strcpy(state, "not work");
    }

    LOG_INFO("Publishing state: %s\n", state);
    mqtt_publish(MQTT_TOPIC, state);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void)
{
  PROCESS_PAUSE();
  return 0;
}
