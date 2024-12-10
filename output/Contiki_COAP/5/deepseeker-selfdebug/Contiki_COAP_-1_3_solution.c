
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-socket.h"

#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_link_t resource;
static struct coap_endpoint_t server_ep;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }

  char payload[64];
  int len = coap_get_payload(response, (uint8_t *)payload);

  if(len > 0) {
    payload[len] = '\0';
    printf("Response received: %s\n", payload);
  }
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;
  static coap_message_t request;

  PROCESS_BEGIN();

  // Initialize the CoAP endpoint
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  server_ep.port = SERVER_PORT;

  // Create a CoAP request
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", 16);

  // Send the request
  coap_send_request(&server_ep, &request, response_handler);

  // Set a timer to resend the request if no response is received
  etimer_set(&et, CLOCK_SECOND * 2);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    coap_send_request(&server_ep, &request, response_handler);
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}
