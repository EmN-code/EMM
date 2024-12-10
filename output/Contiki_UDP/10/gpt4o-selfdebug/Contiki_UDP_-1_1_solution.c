#include <stdlib.h>
#include <string.h>
#include "sys/etimer.h"
#include "sys/clock.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/simple-udp.h"
#include "contiki.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)
#define DEVICE_STATE "work"

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;
static struct etimer periodic_timer;

PROCESS_THREAD(udp_client_process, ev, data)
{
  PROCESS_BEGIN();

  // Initialize UDP connection
  simple_udp_register(&udp_conn, UIP_HTONS(UDP_SERVER_PORT), NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);

  // Set periodic timer to send the state every 5 seconds
  etimer_set(&periodic_timer, SEND_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    // Send device state to the server
    simple_udp_send(&udp_conn, DEVICE_STATE, strlen(DEVICE_STATE));

    // Reset the timer
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}

int main() {
  process_start(&udp_client_process, NULL);
  return 0;
}