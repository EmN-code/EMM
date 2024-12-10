#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "sys/etimer.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "contiki.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_client_process, ev, data) {
  static struct etimer timer;
  static bool device_state = true; // Assume "true" means "work" and "false" means "not work"
  uip_ipaddr_t dest_ipaddr;

  PROCESS_BEGIN();

  uiplib_ipaddrconv(UDP_SERVER_IP, &dest_ipaddr);
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, &dest_ipaddr, UDP_SERVER_PORT, NULL);

  etimer_set(&timer, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if(device_state) {
      simple_udp_send(&udp_conn, "work", 4);
    } else {
      simple_udp_send(&udp_conn, "not work", 8);
    }

    device_state = !device_state; // Toggle device state for demonstration purposes
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  process_init();
  process_start(&udp_client_process, NULL);
  return 0;
}