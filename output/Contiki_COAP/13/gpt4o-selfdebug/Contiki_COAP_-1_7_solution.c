#include "net/ipv6/uip-nameserver.h"
#include "net/mac/framer/framer.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

#include <stdio.h>
#include <string.h>

// Let's simulate sending CoAP POST from NRF52840
#define SERVER_EP "coap://[47.102.103.1]:5683"
#define POST_DATA "Hello, COAP Cloud"

PROCESS(coap_example_client, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client);

PROCESS_THREAD(coap_example_client, ev, data) {
  PROCESS_BEGIN();

  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // Array size of one

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  coap_set_payload(request, (uint8_t *)POST_DATA, sizeof(POST_DATA) - 1);

  printf("Client sending CoAP POST to %s\n", SERVER_EP);

  COAP_BLOCKING_REQUEST(&server_ep, request,
                        chunk_handler());

  PROCESS_END();
}

void chunk_handler(const coap_response_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %*s\n", len, (char *)chunk);
}

int main() {
    // Initialization code if required can be placed here.
    printf("Starting CoAP client example on NRF52840...\n");
    process_start(&coap_example_client, NULL);
    while(1) {
        // Main loop logic if required
    }
    return 0;
}