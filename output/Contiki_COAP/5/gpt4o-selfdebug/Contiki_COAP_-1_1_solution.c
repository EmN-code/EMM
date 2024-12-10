#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static coap_endpoint_t server_ep;
static coap_message_t request[1]; /* This way the packet can be treated as pointer as usual. */

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  /* Initialize the CoAP engine */
  coap_engine_init();

  /* Set the server address */
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

  /* Initialize the CoAP request */
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");

  /* Set the payload */
  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  while(1) {
    /* Send the CoAP request */
    COAP_BLOCKING_REQUEST(&server_ep, request, NULL);
    printf("CoAP POST request sent\n");

    /* Wait for some time before sending the request again */
    etimer_set(&etimer, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etimer));
  }

  PROCESS_END();
}
