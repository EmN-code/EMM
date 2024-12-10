#include "net/ipv6/uip.h"
#include "net/ipv6/simple-udp.h"

#include "contiki.h"
#include <stdio.h>

#define UDP_PORT 20001
#define SERVER_IP "47.102.103.1"
#define INTERVAL (CLOCK_SECOND * 5)

PROCESS(nrf52840_udp_client_process, "NRF52840 UDP Client");
AUTOSTART_PROCESSES(&nrf52840_udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(nrf52840_udp_client_process, ev, data)
{
  static struct etimer timer;
  static char device_state[] = "work";

  PROCESS_BEGIN();

  // Initialize the UDP connection
  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);

  // Set periodic timer
  etimer_set(&timer, INTERVAL);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Toggle device state
    if (strcmp(device_state, "work") == 0) {
        strcpy(device_state, "not work");
    } else {
        strcpy(device_state, "work");
    }

    // Send message to server
    uip_ipaddr_t server_ipaddr;
    uiplib_ipaddrconv(SERVER_IP, &server_ipaddr);
    simple_udp_sendto(&udp_conn, device_state, strlen(device_state), &server_ipaddr);

    printf("Device state sent: %s\n", device_state);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}