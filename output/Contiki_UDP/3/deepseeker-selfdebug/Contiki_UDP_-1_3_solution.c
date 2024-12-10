
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "sys/etimer.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"

static struct simple_udp_connection udp_conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  PROCESS_BEGIN();

  // Register the UDP connection
  simple_udp_register(&udp_conn, 0, NULL, SERVER_PORT, NULL);

  // Set the timer for 5 seconds
  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Reset the timer
    etimer_reset(&et);

    // Prepare the message
    static char message[] = DEVICE_STATE;

    // Send the message to the server
    uip_ipaddr_t server_addr;
    uip_ipaddr(&server_addr, SERVER_IP);
    simple_udp_sendto(&udp_conn, message, sizeof(message) - 1, &server_addr);
  }

  PROCESS_END();
}
