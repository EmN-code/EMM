#include <stdio.h>
#include <stdint.h>
#include "uip.h"
#include "simple-udp.h"
#include "clock.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define CLIENT_PORT 1234

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client example");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  static char* message_work = "work";
  static char* message_not_work = "not work";
  static uint8_t state = 0;  // 0: not work, 1: work

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, CLIENT_PORT, NULL, SERVER_PORT, NULL);

  etimer_set(&periodic_timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, 47, 102, 103, 1);

    if (state) {
      simple_udp_sendto(&udp_conn, message_work, sizeof(message_work), &server_ip);
    } else {
      simple_udp_sendto(&udp_conn, message_not_work, sizeof(message_not_work), &server_ip);
    }

    state = !state;  // Toggle state for demonstration purposes

    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
