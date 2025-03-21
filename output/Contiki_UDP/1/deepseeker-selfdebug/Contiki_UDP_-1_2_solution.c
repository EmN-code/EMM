
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/udp-socket.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_state(const char *state) {
  static char buf[32];
  int len = snprintf(buf, sizeof(buf), "Device state: %s", state);
  uip_udp_packet_send(conn, buf, len);
}

PROCESS_THREAD(udp_client_process, ev, data) {
  static char state[] = "work";

  PROCESS_BEGIN();

  // Initialize UDP connection
  conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(conn, UIP_HTONS(0));
  uip_ipaddr_t ipaddr;
  uip_ipaddr(&ipaddr, UDP_SERVER_IP, UDP_SERVER_PORT);
  udp_connect(conn, &ipaddr, UIP_HTONS(UDP_SERVER_PORT));

  // Send state every 5 seconds
  etimer_set(&et, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    send_state(state);
    etimer_reset(&et);
  }

  PROCESS_END();
}
