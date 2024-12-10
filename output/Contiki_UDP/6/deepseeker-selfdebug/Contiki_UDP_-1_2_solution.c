
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define DEVICE_STATE_MESSAGE_LENGTH 4
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *udp_conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void udp_send_state(void) {
  uip_ipaddr_t server_ip;
  uip_ipaddr(&server_ip, UDP_SERVER_IP);
  uip_udp_packet_sendto(udp_conn, DEVICE_STATE_MESSAGE, DEVICE_STATE_MESSAGE_LENGTH, &server_ip, UIP_HTONS(UDP_SERVER_PORT));
}

PROCESS_THREAD(udp_client_process, ev, data) {
  PROCESS_BEGIN();

  // Initialize UDP connection
  udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(udp_conn, UIP_HTONS(0));

  // Set up periodic timer
  etimer_set(&et, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Send device state
    udp_send_state();

    // Reset timer
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void) {
  // Initialize Contiki
  contiki_init();

  // Start the UDP client process
  process_start(&udp_client_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
