#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)
#define DEVICE_STATE "work"
#define BUFFER_SIZE 128

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct uip_udp_conn *client_conn;
static uip_ipaddr_t server_ipaddr;

PROCESS_THREAD(udp_client_process, ev, data) {
  static struct etimer periodic_timer;
  static char buf[BUFFER_SIZE];

  PROCESS_BEGIN();

  uip_ip6addr(&server_ipaddr, 0x2f00, 0, 0, 0, 0, 0, 0x67, 0x101);
  client_conn = uip_udp_new(&server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));

  if (!client_conn) {
    printf("Failed to create UDP connection.");
    PROCESS_EXIT();
  }

  uip_udp_bind(client_conn, UIP_HTONS(5678));

  etimer_set(&periodic_timer, SEND_INTERVAL);
  while (1) {
    PROCESS_YIELD();

    if (etimer_expired(&periodic_timer)) {
      sprintf(buf, "Device state: %s", DEVICE_STATE);
      uip_udp_packet_sendto(client_conn, buf, strlen(buf),
                            &server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));
      printf("Sent message: %s\n", buf);
      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}