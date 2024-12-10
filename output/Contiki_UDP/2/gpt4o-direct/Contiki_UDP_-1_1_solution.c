#include "net/ipv6/simple-udp.h"
#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

PROCESS(udp_client_process, "UDP Client");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  uip_ipaddr_t server_ipaddr;
  const char *device_state_work = "work";
  const char *device_state_not_work = "not work";
  static int work_state = 1;

  PROCESS_BEGIN();

  // Convert server IPv4 string to an IPv6 address placeholder
  uip_ip6addr(&server_ipaddr, 0, 0, 0, 0, 0, 0xFFFF, server_ip4[3], server_ip4[2], server_ip4[1], server_ip4[0]);

  // Register a UDP connection
  simple_udp_register(&udp_conn, SERVER_PORT, NULL, SERVER_PORT, NULL);

  // Set the timer for sending intervals
  etimer_set(&timer, SEND_INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Send the device state to the server
    if(work_state) {
      printf("Sending state: %s\n", device_state_work);
      simple_udp_sendto(&udp_conn, device_state_work, strlen(device_state_work) + 1, &server_ipaddr);
    } else {
      printf("Sending state: %s\n", device_state_not_work);
      simple_udp_sendto(&udp_conn, device_state_not_work, strlen(device_state_not_work) + 1, &server_ipaddr);
    }

    // Toggle the state for demonstration
    work_state = !work_state;

    // Reset the timer to wait another five seconds
    etimer_reset(&timer);
  }

  PROCESS_END();
}