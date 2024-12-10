#include <stdio.h>
#include <stdlib.h>
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "coap-request-state.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define POST_MESSAGE "Hello, COAP Cloud"

PROCESS(coap_example_process, "CoAP Example Process");
AUTOSTART_PROCESSES(&coap_example_process);

static struct etimer et;

PROCESS_THREAD(coap_example_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND * 2);

  printf("Starting CoAP client example\n");

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_reset(&et);

    coap_init_engine();

    coap_packet_t request[1];
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, URI_PATH);
    coap_set_payload(request, (uint8_t *)POST_MESSAGE, sizeof(POST_MESSAGE) - 1);

    uip_ipaddr_t server_ipaddr;
    uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);
    COAP_BLOCKING_REQUEST(&server_ipaddr, SERVER_PORT, request, NULL);
    printf("CoAP request sent\n");
  }

  PROCESS_END();
}

int main() {
  printf("This is a placeholder for the main function. In Contiki, the process is defined and started using macros.");
  return 0;
}