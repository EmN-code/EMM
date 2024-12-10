#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>

#define UDP_SERVER_ADDRESS "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_send_process, "UDP Send Process");
AUTOSTART_PROCESSES(&udp_send_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_send_process, ev, data)
{
  static struct etimer timer;
  static char *message = "work";
  uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  // Set the server IP address
  uiplib_ipaddrconv(UDP_SERVER_ADDRESS, &server_ipaddr);

  // Initialize UDP connection
  simple_udp_register(&udp_conn, 0, &server_ipaddr, 
                      UDP_SERVER_PORT, NULL);

  // Set the timer interval
  etimer_set(&timer, INTERVAL);

  while(1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Send UDP packet with device state
    simple_udp_send(&udp_conn, message, strlen(message));
    printf("Sent state: %s to server\n", message);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  // Start processes defined in the application
  process_start(&udp_send_process, NULL);
  // Enter Contiki OS scheduler
  while(1) {
    process_run();
    etimer_request_poll();
  }
  return 0;
}