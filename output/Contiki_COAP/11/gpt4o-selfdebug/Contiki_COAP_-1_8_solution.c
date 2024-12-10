#include <stdio.h>
#include <stdlib.h>
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];

  printf("Starting CoAP client example\n");

  /* Prepare the server address */
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_endpoint);

  /* Init CoAP request message */
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");

  const char *msg = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, strlen(msg));

  printf("Sending request to %s\n", SERVER_EP);

  COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

  printf("Request sent\n");

  PROCESS_END();
}