
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "simple-udp.h"
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct simple_udp_connection udp_conn;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void
coap_response_handler(coap_message_t *response)
{
  if(response == NULL) {
    printf("No response received\n");
    return;
  }

  const uint8_t *payload;
  size_t payload_len;
  if(coap_get_payload(response, &payload, &payload_len)) {
    printf("Response: %.*s\n", (int)payload_len, payload);
  }
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, 0, NULL, 0, NULL);

  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  coap_endpoint_set_port(&server_ep, SERVER_PORT);

  while(1) {
    static struct etimer et;
    etimer_set(&et, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, URI_PATH);
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 16);

    COAP_BLOCKING_REQUEST(&server_ep, request, coap_response_handler);
  }

  PROCESS_END();
}

int main()
{
  process_start(&coap_client_process, NULL);
  return 0;
}
