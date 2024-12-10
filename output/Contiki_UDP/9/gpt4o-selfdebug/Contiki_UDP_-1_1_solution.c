#include "simple-udp.h"
#include "net/netstack.h"
#include "contiki.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (CLOCK_SECOND * 5)  // Send every 5 seconds

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  static char *device_state = "work"; // or "not work"

  PROCESS_BEGIN();

  // Initialize UDP connection
  simple_udp_register(&udp_conn, SERVER_PORT, NULL, SERVER_PORT, NULL);

  // Set the timer
  etimer_set(&timer, INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    // Send the device state to the server
    simple_udp_sendto(&udp_conn, device_state, strlen(device_state), &server_ipaddr);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}
