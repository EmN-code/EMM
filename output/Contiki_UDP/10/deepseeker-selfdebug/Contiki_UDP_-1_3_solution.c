
#include "contiki.h"
#include "contiki-net.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define UDP_CLIENT_PORT 12345

static struct uip_udp_conn *udp_conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

void send_udp_message(const char *message) {
  uip_ipaddr_t server_ip;
  uip_ipaddr(&server_ip, UDP_SERVER_IP);
  uip_udp_packet_send(udp_conn, message, strlen(message));
}

PROCESS_THREAD(udp_client_process, ev, data) {
  static char message[] = "work";
  static int state = 0;

  PROCESS_BEGIN();

  udp_conn = udp_new(NULL, UIP_HTONS(UDP_CLIENT_PORT), NULL);
  udp_bind(udp_conn, UIP_HTONS(UDP_CLIENT_PORT));

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if(state == 0) {
      strcpy(message, "work");
      state = 1;
    } else {
      strcpy(message, "not work");
      state = 0;
    }

    send_udp_message(message);
    etimer_reset(&et);
  }

  PROCESS_END();
}
