
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MESSAGE "work"

static struct simple_udp_connection udp_conn;

PROCESS(udp_sender_process, "UDP Sender Process");
AUTOSTART_PROCESSES(&udp_sender_process);

static void udp_callback(struct simple_udp_connection *c,
                         const uip_ipaddr_t *sender_addr,
                         uint16_t sender_port,
                         const uip_ipaddr_t *receiver_addr,
                         uint16_t receiver_port,
                         const uint8_t *data,
                         uint16_t datalen)
{
  // Callback function for handling UDP responses (if needed)
}

PROCESS_THREAD(udp_sender_process, ev, data)
{
  static struct etimer et;
  uip_ipaddr_t server_ip;

  PROCESS_BEGIN();

  // Initialize UDP connection
  simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, udp_callback);

  // Set the server IP address
  uip_ipaddr_copy(&server_ip, uip_ip6addr(UDP_SERVER_IP, 0, 0, 0, 0, 0, 0, 0));

  // Set the timer for 5 seconds
  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Send the device state message
    simple_udp_sendto(&udp_conn, DEVICE_STATE_MESSAGE, sizeof(DEVICE_STATE_MESSAGE) - 1, &server_ip);

    // Reset the timer
    etimer_reset(&et);
  }

  PROCESS_END();
}
