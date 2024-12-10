
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv4/uip-ds6.h"
#include "net/ipv4/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work" // or "not work"

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_udp_message(const char *message) {
  uip_ipaddr_t server_ip;
  uip_ipaddr(&server_ip, 47, 102, 103, 1);
  uip_udp_packet_send(conn, message, strlen(message));
}

PROCESS_THREAD(udp_client_process, ev, data) {
  PROCESS_BEGIN();

  // Create a UDP connection
  conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(conn, UIP_HTONS(UDP_SERVER_PORT));

  // Set the timer for 5 seconds
  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Send the device state
    send_udp_message(DEVICE_STATE);

    // Reset the timer
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&udp_client_process, NULL);
  return 0;
}
