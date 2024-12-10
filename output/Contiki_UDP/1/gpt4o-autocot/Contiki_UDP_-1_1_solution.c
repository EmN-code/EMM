#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (CLOCK_SECOND * 5)

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;
  static char *device_state = "work";

  PROCESS_BEGIN();

  // Initialize UDP connection
  simple_udp_register(&udp_conn, 0, NULL, SERVER_PORT, NULL);

  etimer_set(&timer, INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Send device state to the server
    simple_udp_send(&udp_conn, device_state, sizeof(device_state));
    printf("Sent device state: %s\n", device_state);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&udp_client_process, NULL);
  return 0;
}