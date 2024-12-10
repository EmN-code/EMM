
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/coap/coap.h"
#include "sys/etimer.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct uip_udp_conn *coap_conn;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_response_handler(coap_message_t *response) {
  const uint8_t *payload;
  uint16_t payload_len;

  if(response == NULL) {
    printf("No response received\n");
    return;
  }

  coap_get_payload(response, &payload, &payload_len);
  printf("Received response: %.*s\n", payload_len, payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;
  static coap_message_t request;
  static uint8_t buffer[128];

  PROCESS_BEGIN();

  coap_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(coap_conn, UIP_HTONS(0));

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", 16);

    if(coap_send_request(&request, coap_conn, SERVER_IP, SERVER_PORT, coap_response_handler) < 0) {
      printf("Failed to send CoAP request\n");
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}
