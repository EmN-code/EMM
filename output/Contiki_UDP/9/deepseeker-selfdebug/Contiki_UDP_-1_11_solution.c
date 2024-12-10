
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv4/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *udp_conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_device_state() {
  static char buf[32];
  strncpy(buf, DEVICE_STATE_MESSAGE, sizeof(buf));
  uip_ipaddr_t ipaddr;
  uip_ipaddr(&ipaddr, UDP_SERVER_IP);
  uip_udp_packet_sendto(udp_conn, buf, strlen(buf), &ipaddr, UIP_HTONS(UDP_SERVER_PORT));
}

PROCESS_THREAD(udp_client_process, ev, data) {
  PROCESS_BEGIN();

  udp_conn = udp_new(NULL, 0, NULL);
  udp_bind(udp_conn, UIP_HTONS(0));

  etimer_set(&et, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    send_device_state();
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&udp_client_process, NULL);
  return 0;
}
