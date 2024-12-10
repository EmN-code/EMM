
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_state(const char *state) {
  uip_ipaddr_t addr;
  uip_ipaddr(&addr, 47, 102, 103, 1);
  uip_udp_packet_send(conn, state, strlen(state));
}

PROCESS_THREAD(udp_client_process, ev, data) {
  static char state[] = "work";

  PROCESS_BEGIN();

  conn = uip_udp_new(&addr, UIP_HTONS(UDP_SERVER_PORT));
  if(conn == NULL) {
    printf("Failed to create UDP connection\n");
    PROCESS_EXIT();
  }

  etimer_set(&et, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    send_state(state);
    printf("Sent state: %s\n", state);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&udp_client_process, NULL);
  return 0;
}
