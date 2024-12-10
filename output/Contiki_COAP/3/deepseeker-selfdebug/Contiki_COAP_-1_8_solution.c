
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-nameserver.h"

#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct etimer et;
static coap_endpoint_t server_ep;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }

  const uint8_t *payload = NULL;
  size_t payload_len = 0;

  if(coap_get_payload(response, &payload, &payload_len)) {
    printf("Response received: %.*s\n", (int)payload_len, payload);
  }
}

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  static coap_message_t request;

  // Initialize the CoAP endpoint
  coap_endpoint_init(&server_ep, NULL, 0, 0);
  coap_endpoint_set_ip(&server_ep, SERVER_IP);
  coap_endpoint_set_port(&server_ep, SERVER_PORT);

  // Create a CoAP POST request
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  const char *payload = "Hello, COAP Cloud";
  coap_set_payload(&request, (uint8_t *)payload, strlen(payload));

  // Send the request and wait for the response
  coap_blocking_send(&server_ep, &request, response_handler);

  etimer_set(&et, CLOCK_SECOND * 5);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  PROCESS_END();
}
