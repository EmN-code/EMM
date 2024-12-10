#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "sys/process.h"
#include "simple-udp.h"
#include <stdio.h>

#define UDP_PORT 20001
#define SERVER_IP "47.102.103.1"

PROCESS(udp_sender_process, "UDP sender process");
AUTOSTART_PROCESSES(&udp_sender_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_sender_process, ev, data)
{
  static struct etimer timer;
  static char msg[20];
  uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);
  simple_udp_register(&udp_conn, UDP_PORT, &server_ipaddr, UDP_PORT, NULL);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    sprintf(msg, "work"); // Replace "work" based on actual device state if needed
    simple_udp_send(&udp_conn, msg, strlen(msg));
    printf("Sent message: %s\n", msg);
  }
  
  PROCESS_END();
}