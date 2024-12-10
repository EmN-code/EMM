#include "contiki-net.h"
#include "contiki-lib.h"
#include "coap-timer.h"
#include "coap-request-state.h"
#include "coap-observe-client.h"
#include <stdio.h>

PROCESS(coap_example_client_process, "COAP Client Example Process");
AUTOSTART_PROCESSES(&coap_example_client_process);

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];  // This way we can avoid dynamic memory allocation

PROCESS_THREAD(coap_example_client_process, ev, data)
{
  static struct etimer timer;
  coap_udp_init();

  PROCESS_BEGIN();

  // Setting the server address
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    printf("Sending CoAP POST request\n");

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 17);

    COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

    printf("Request sent\n");
  }

  PROCESS_END();
}