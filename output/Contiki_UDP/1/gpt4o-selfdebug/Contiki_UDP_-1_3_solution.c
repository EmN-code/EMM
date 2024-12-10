#include "sys/etimer.h"
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/log.h"

#define LOG_MODULE "UDP Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define LOCAL_PORT 1234

#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"

static struct uip_udp_conn *client_conn;
static struct etimer et;

PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static char state_msg[15];
  PROCESS_BEGIN();

  // Create a new connection with remote peer
  uip_ipaddr_t dest_ipaddr;
  uip_ip6addr(&dest_ipaddr, UDP_SERVER_IP);
  client_conn = udp_new(&dest_ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
  if (client_conn == NULL) {
      LOG_ERR("Failed to create UDP connection");
      PROCESS_EXIT();
  }
  udp_bind(client_conn, UIP_HTONS(LOCAL_PORT));

  // Set a timer to send the device state every 5 seconds
  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&et)) {
      // Alternate device state
      if(strcmp(state_msg, STATE_WORK) == 0) {
        strcpy(state_msg, STATE_NOT_WORK);
      } else {
        strcpy(state_msg, STATE_WORK);
      }

      // Send the device state to the server
      uip_udp_packet_send(client_conn, state_msg, strlen(state_msg));
      LOG_INFO("Sent state: %s\n", state_msg);
      
      // Reset timer
      etimer_reset(&et);
    }
  }

  PROCESS_END();
}