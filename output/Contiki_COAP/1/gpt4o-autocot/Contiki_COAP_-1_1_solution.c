#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

static coap_endpoint_t server_ep;
static coap_message_t request[1];
static char *msg = "Hello, COAP Cloud";

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  coap_init_engine();

  /* Configure the server endpoint */
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  /* Prepare the request packet */
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
  coap_set_header_uri_path(request, "/coap/test");
  coap_set_payload(request, (uint8_t *)msg, strlen(msg));

  /* Issue the request asynchronously */
  COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

  PROCESS_END();
}

#define SERVER_EP "coap://47.102.103.1:5683"