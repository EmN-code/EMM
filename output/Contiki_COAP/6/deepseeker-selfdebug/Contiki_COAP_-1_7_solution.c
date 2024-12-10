
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "sys/log.h"

#define LOG_MODULE "CoAP Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

#define COAP_PAYLOAD "Hello, COAP Cloud"

static struct simple_udp_connection coap_conn;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void
coap_response_handler(struct simple_udp_connection *c,
                      const uip_ipaddr_t *sender_addr,
                      uint16_t sender_port,
                      const uip_ipaddr_t *receiver_addr,
                      uint16_t receiver_port,
                      const uint8_t *data,
                      uint16_t datalen)
{
  LOG_INFO("Received response from ");
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n");
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  static struct etimer et;
  static uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  LOG_INFO("Starting CoAP client\n");

  simple_udp_register(&coap_conn, UIP_HTONS(COAP_SERVER_PORT), NULL,
                      UIP_HTONS(COAP_SERVER_PORT), coap_response_handler);

  uip_ipaddr_from_str(&server_ipaddr, COAP_SERVER_IP);

  etimer_set(&et, CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    LOG_INFO("Sending CoAP POST request to ");
    LOG_INFO_6ADDR(&server_ipaddr);
    LOG_INFO_("\n");

    uip_udp_packet_sendto(&coap_conn, COAP_PAYLOAD, strlen(COAP_PAYLOAD),
                          &server_ipaddr, UIP_HTONS(COAP_SERVER_PORT));

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}
