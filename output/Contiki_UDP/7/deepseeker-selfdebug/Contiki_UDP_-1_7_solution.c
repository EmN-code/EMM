
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include "sys/log.h"

#define LOG_MODULE "Device State"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 12345
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_ADDR "47.102.103.1"

static struct simple_udp_connection udp_conn;
static struct etimer et;

PROCESS(device_state_process, "Device State Process");
AUTOSTART_PROCESSES(&device_state_process);

static void udp_rx_callback(struct simple_udp_connection *c,
                            const uip_ipaddr_t *sender_addr,
                            uint16_t sender_port,
                            const uip_ipaddr_t *receiver_addr,
                            uint16_t receiver_port,
                            const uint8_t *data,
                            uint16_t datalen)
{
  LOG_INFO("Received response from server\n");
}

PROCESS_THREAD(device_state_process, ev, data)
{
  static char state[] = "work";
  static uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_rx_callback);

  uip_ipaddr_from_str(&server_ipaddr, UDP_SERVER_ADDR);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    simple_udp_sendto(&udp_conn, state, sizeof(state), &server_ipaddr);

    etimer_reset(&et);
  }

  PROCESS_END();
}
