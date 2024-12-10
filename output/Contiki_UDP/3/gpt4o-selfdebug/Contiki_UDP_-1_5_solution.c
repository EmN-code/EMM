#include "uip.h"
#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>

PROCESS(nrf52840_udp_client_process, "NRF52840 UDP Client Process");
AUTOSTART_PROCESSES(&nrf52840_udp_client_process);

static struct uip_udp_conn *client_conn;
static uip_ipaddr_t server_ip;

PROCESS_THREAD(nrf52840_udp_client_process, ev, data)
{
  static struct etimer send_timer;
  static char buf[20];
  static int state = 0; // 0: not work, 1: work

  PROCESS_BEGIN();

  uip_ipaddr(&server_ip, 47, 102, 103, 1);
  client_conn = udp_new(&server_ip, UIP_HTONS(20001), NULL);

  if (!client_conn) {
    printf("Failed to connect to server\n");
    PROCESS_EXIT();
  }

  etimer_set(&send_timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));

    // Toggle state
    state = !state;
    snprintf(buf, sizeof(buf), "State: %s", state ? "work" : "not work");

    // Send the state
    uip_udp_packet_send(client_conn, buf, strlen(buf));
    printf("Sent: %s\n", buf);

    // Reset the timer
    etimer_reset(&send_timer);
  }

  PROCESS_END();
}