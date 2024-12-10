
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_link_t resource;
static struct coap_endpoint_t server_endpoint;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }

  uint8_t statuscode_buf[3];
  coap_get_statuscode(response, statuscode_buf);
  printf("Response received: %02x%02x\n", statuscode_buf[0], statuscode_buf[1]);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;
  static coap_message_t request;

  PROCESS_BEGIN();

  // Initialize the CoAP endpoint
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_endpoint);
  server_endpoint.port = SERVER_PORT;

  // Create a CoAP request
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(&request, URI_PATH);
  coap_set_payload(&request, "Hello, COAP Cloud", 16);

  // Send the CoAP request
  COAP_BLOCKING_REQUEST(&server_endpoint, &request, response_handler);

  etimer_set(&et, CLOCK_SECOND * 5);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}
