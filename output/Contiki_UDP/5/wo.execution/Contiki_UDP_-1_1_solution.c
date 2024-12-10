#include <string.h>
#include <stdio.h>
#include "simple-udp.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "sys/process.h"
#include "sys/clock.h"

PROCESS(udp_sender_process, "UDP sender process");
AUTOSTART_PROCESSES(&udp_sender_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_sender_process, ev, data)
{
  static struct etimer timer;
  static char msg[] = "work";
  static uip_ipaddr_t dest_ipaddr;

  PROCESS_BEGIN();

  // Set the destination IP address
  uip_ip6addr(&dest_ipaddr, 0x1002, 0xff47, 0xfffd, 0xff19, 102, 103, 1, 20001);

  // Register UDP connection
  simple_udp_register(&udp_conn, 0, NULL, 20001, NULL);

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Send the message to the UDP server
    simple_udp_sendto(&udp_conn, msg, strlen(msg), &dest_ipaddr);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}