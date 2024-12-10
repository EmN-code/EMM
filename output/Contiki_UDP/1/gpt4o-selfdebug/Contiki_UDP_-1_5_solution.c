#include <net/ip/uip.h>
#include <contiki_net.h>
#include <contiki.h>

#include <string.h>
#include <stdio.h>

#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *client_conn;
static char device_state[16];

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  // Convert the server IP address
  uiplib_ipaddrconv(SERVER_IP, &server_ipaddr);

  // Create a new UDP connection
  client_conn = udp_new(&server_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);

  if (client_conn == NULL) {
    printf("No UDP connection available, exiting the process\n");
    PROCESS_EXIT();
  }

  udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT));

  // Set the initial device state
  strcpy(device_state, "work");

  etimer_set(&periodic_timer, SEND_INTERVAL);

  while (1) {
    PROCESS_YIELD();

    if (ev == PROCESS_EVENT_TIMER) {
      // Toggle device state
      if (strcmp(device_state, "work") == 0) {
        strcpy(device_state, "not work");
      } else {
        strcpy(device_state, "work");
      }

      // Send device state to the server
      printf("Sending device state: %s\n", device_state);
      uip_udp_packet_send(client_conn, device_state, strlen(device_state));

      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}