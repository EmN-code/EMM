
#include "contiki.h"
#include "coap.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }

  uint8_t *payload = NULL;
  uint16_t payload_len = 0;

  if(coap_get_payload(response, &payload, &payload_len)) {
    printf("Response received: %.*s\n", payload_len, payload);
  } else {
    printf("No payload in response\n");
  }
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;
  static coap_endpoint_t server_ep;
  static coap_message_t request;
  static uint8_t buffer[128];

  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_endpoint_init(&server_ep);
    coap_endpoint_set_ip(&server_ep, COAP_SERVER_IP);
    coap_endpoint_set_port(&server_ep, COAP_SERVER_PORT);

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", 16);

    if(coap_blocking_send(&server_ep, &request, buffer, sizeof(buffer), coap_response_handler) < 0) {
      printf("Failed to send CoAP request\n");
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}
