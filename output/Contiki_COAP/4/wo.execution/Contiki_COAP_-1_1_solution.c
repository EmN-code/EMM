#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"

#include <stdio.h>
#include <string.h>

#define REMOTE_PORT 5683
const char *uri_path = "/coap/test";
const char *remote_ip = "47.102.103.1";

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

static void
coap_response_handler(coap_message_t *response)
{
  const uint8_t *chunk;

  int len = coap_get_payload(response, &chunk);

  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  uip_ip6addr_t ipaddr;

  printf("CoAP client started\n");

  uip_ip6addr(&ipaddr, 0x2407,0x6600,0x0,0x0,0x0,0x0,0x0,0x1);
  coap_endpoint_parse(&server_endpoint, remote_ip);
  server_endpoint.port = UIP_HTONS(REMOTE_PORT);

  printf("Server endpoint set up\n");

  printf("Sending CoAP POST request\n");

  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_uri_path(request, uri_path);

  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  COAP_BLOCKING_REQUEST(&server_endpoint, request,
                        coap_response_handler);

  printf("Request sent\n");

  PROCESS_END();
}