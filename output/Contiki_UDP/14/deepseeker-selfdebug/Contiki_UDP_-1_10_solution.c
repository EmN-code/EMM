
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-nd6.h"
#include "net/ip/uip-udp-packet.h"
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

void udp_send_state(void) {
  static char buf[32];
  sprintf(buf, "%s", DEVICE_STATE_MESSAGE);
  uip_ipaddr_t server_ip;
  uip_ipaddr(&server_ip, 47, 102, 103, 1);
  uip_udp_packet_sendto(udp_conn, buf, strlen(buf), &server_ip, UIP_HTONS(UDP_SERVER_PORT));
}

PROCESS_THREAD(udp_client_process, ev, data) {
  PROCESS_BEGIN();

  udp_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(udp_conn, UIP_HTONS(0));

  etimer_set(&et, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    udp_send_state();
    etimer_reset(&et);
  }

  PROCESS_END();
}
