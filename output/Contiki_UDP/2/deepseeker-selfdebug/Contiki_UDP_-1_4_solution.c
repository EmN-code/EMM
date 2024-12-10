
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE "work"

static struct uip_udp_conn *conn;

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static void
send_udp_message(const char *message)
{
  uip_ipaddr_t server_ip;
  uip_ipaddr(&server_ip, 47, 102, 103, 1);

  uip_udp_packet_send(conn, message, strlen(message));
}

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  conn = uip_udp_new(&uip_all_zero_ipaddr, UIP_HTONS(UDP_SERVER_PORT));
  if(conn == NULL) {
    printf("Failed to create UDP connection\n");
    PROCESS_EXIT();
  }

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    send_udp_message(DEVICE_STATE);
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&udp_client_process, NULL);
  return 0;
}
