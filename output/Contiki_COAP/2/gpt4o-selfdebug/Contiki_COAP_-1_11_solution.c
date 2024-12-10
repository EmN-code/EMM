#include "net/ipv6/uip-nameserver.h"
#include "coap-observe-client.h"
#include "coap-request-state.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "contiki.h"

// Standard library includes
#include <stdio.h>
#include <string.h>

// Setup the Contiki CoAP client process
PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

// Specify the server address and port
static coap_packet_t request[1]; // This needs to be allocated
static const char *server_ip = "47.102.103.1";
static const uint16_t server_port = 5683;

// The implementation of the process
PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  // Prepare the CoAP request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  const char *msg = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, strlen(msg));

  // Send the CoAP request
  COAP_BLOCKING_REQUEST(&server_ip, request, NULL);

  PROCESS_END();
}