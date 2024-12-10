#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "sys/etimer.h"
#include "contiki.h"
#include "os/dev/leds.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct etimer periodic_timer;
static struct uip_udp_conn *client_conn;

PROCESS_THREAD(udp_client_process, ev, data){
  PROCESS_BEGIN();

  etimer_set(&periodic_timer, CLOCK_SECOND * 5);
  
  uip_ipaddr_t server_ipaddr;
  uip_ip6addr(&server_ipaddr, 0x2f66, 0x6701, 0, 0, 0, 0, 0, 1);
  client_conn = uip_udp_new(&server_ipaddr, UIP_HTONS(UDP_SERVER_PORT));

  if(client_conn == NULL) {
    PROCESS_EXIT();
  }

  uip_udp_bind(client_conn, UIP_HTONS(0));

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    char *msg = "work";
    uip_send(msg, strlen(msg));
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}