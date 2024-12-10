
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "dev/serial-line.h"
#include "sys/etimer.h"
#include "sys/process.h"
#include "sys/log.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"

static struct uip_udp_conn *udp_conn;
static struct etimer et;
static char device_state[10];

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static void
udp_send(const char *data)
{
  uip_ipaddr_t ipaddr;
  uip_ipaddr(&ipaddr, UDP_SERVER_IP);
  uip_udp_packet_send(udp_conn, data, strlen(data));
}

PROCESS_THREAD(udp_client_process, ev, data)
{
  PROCESS_BEGIN();

  // Initialize the UDP connection
  udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  uip_ipaddr_t ipaddr;
  uip_ipaddr(&ipaddr, UDP_SERVER_IP);
  udp_bind(udp_conn, UIP_HTONS(UDP_SERVER_PORT));
  udp_connect(udp_conn, &ipaddr, UIP_HTONS(UDP_SERVER_PORT));

  // Set the initial device state
  strcpy(device_state, DEVICE_STATE_WORK);

  // Set the timer for 5 seconds
  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Send the device state to the UDP server
    udp_send(device_state);

    // Toggle the device state
    if(strcmp(device_state, DEVICE_STATE_WORK) == 0) {
      strcpy(device_state, DEVICE_STATE_NOT_WORK);
    } else {
      strcpy(device_state, DEVICE_STATE_WORK);
    }

    // Reset the timer for the next 5 seconds
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize the Contiki system
  process_init();
  process_start(&udp_client_process, NULL);
  autostart_start(autostart_processes);

  // Main loop
  while(1) {
    process_run();
  }

  return 0;
}
