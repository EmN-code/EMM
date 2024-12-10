#include "simple-udp.h"
#include "net/ip/simple-udp.h"
#include "contiki-net.h"
#include "contiki.h"

#include <stdio.h>
#include <string.h>

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_IP "47.102.103.1"

static struct simple_udp_connection udp_conn;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  static char message[32];
  uip_ipaddr_t server_ipaddr;
  
  PROCESS_BEGIN();

  // Set up the UDP connection
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

  // Convert the server IP address from string
  uiplib_ipaddrconv(UDP_SERVER_IP, &server_ipaddr);

  while(1) {
    // Wait for the timer event which triggers every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Prepare the device state message
    strncpy(message, "work", sizeof(message));

    // Send the message to the UDP server
    simple_udp_sendto(&udp_conn, message, strlen(message), &server_ipaddr);
    printf("Sent to %s: %s\n", UDP_SERVER_IP, message);
  }

  PROCESS_END();
}