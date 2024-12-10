#include <string.h>
#include "net/rime/rime.h"
#include "net/ipv6/simple-udp.h"
#include "os/dev/leds.h"
#include "sys/etimer.h"
#include "contiki.h"

#define UDP_SERVER_ADDRESS "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define LOCAL_PORT 1234

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  static char msg[20];

  PROCESS_BEGIN();

  // Initialize UDP connection
  simple_udp_register(&udp_conn, LOCAL_PORT, NULL, UDP_SERVER_PORT, NULL);

  // Set timer for periodic sending every 5 seconds
  etimer_set(&periodic_timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    // Change the device state as needed
    strcpy(msg, "Device is working"); // Example state message

    // Sending the device state message to the UDP server
    simple_udp_sendto(&udp_conn, msg, strlen(msg) + 1,
                      &ipaddr_any); // Use ipaddr_any to specify the destination address

    // Reset the timer for the next period
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}