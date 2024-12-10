#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

#define SERVER_EP "coap://47.102.103.1:5683"
#define COAP_URI "/coap/test"
#define HELLO_MSG "Hello, COAP Cloud"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  /* This way the packet can be treated as pointer as usual. */

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while(1) {
    printf("Sending CoAP POST request to %s%s\n", SERVER_EP, COAP_URI);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, COAP_URI);
    coap_set_payload(request, (uint8_t *)HELLO_MSG, sizeof(HELLO_MSG) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}
