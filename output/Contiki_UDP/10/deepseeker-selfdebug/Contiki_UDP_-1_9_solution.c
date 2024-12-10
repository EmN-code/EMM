
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include "sys/etimer.h"
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct uip_udp_conn *conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_device_state(const char *state) {
  uip_ipaddr_t server_ip;
  uip_ipaddr(&server_ip, 47, 102, 103, 1);

  uip_udp_packet_sendto(conn, state, strlen(state), &server_ip, UIP_HTONS(UDP_SERVER_PORT));
}

PROCESS_THREAD(udp_client_process, ev, data) {
  static char state[] = "work";

  PROCESS_BEGIN();

  conn = uip_udp_new(NULL, 0);
  if(conn == NULL) {
    PROCESS_EXIT();
  }

  etimer_set(&et, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    send_device_state(state);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&udp_client_process, NULL);
  return 0;
}
