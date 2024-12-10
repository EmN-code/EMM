
#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/coap/coap.h"
#include "sys/log.h"

#define LOG_MODULE "CoAP Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct simple_udp_connection udp_conn;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void
udp_rx_callback(struct simple_udp_connection *c,
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
  static coap_message_t request;
  static coap_message_t response;
  static uint8_t buffer[128];
  static uint8_t payload[] = "Hello, COAP Cloud";

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, UIP_HTONS(SERVER_PORT), NULL, UIP_HTONS(SERVER_PORT), udp_rx_callback);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, payload, sizeof(payload) - 1);

    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, SERVER_IP);

    if(coap_send_request(&request, &server_ip, UIP_HTONS(SERVER_PORT), &response, buffer, sizeof(buffer)) > 0) {
      LOG_INFO("CoAP request sent\n");
    } else {
      LOG_INFO("Failed to send CoAP request\n");
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}
