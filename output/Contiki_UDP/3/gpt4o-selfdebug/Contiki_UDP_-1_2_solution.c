#include "contiki.h"
#include "contiki-net.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5 * CLOCK_SECOND)

static struct etimer timer;
static struct uip_udp_conn *conn;

PROCESS(udp_sender_process, "UDP sender process");
AUTOSTART_PROCESSES(&udp_sender_process);

PROCESS_THREAD(udp_sender_process, ev, data)
{
  PROCESS_BEGIN();
  
  uip_ipaddr_t server_ipaddr;
  uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);

  conn = udp_new(&server_ipaddr, UIP_HTONS(SERVER_PORT), NULL);
  if(!conn) {
    PROCESS_EXIT();
  }

  etimer_set(&timer, SEND_INTERVAL);

  while(1) {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER) {
      char msg[] = "work";
      uip_udp_packet_send(conn, msg, sizeof(msg));

      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}