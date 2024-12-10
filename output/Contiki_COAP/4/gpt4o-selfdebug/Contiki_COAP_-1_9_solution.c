#include "coap-timer.h"
#include "coap-separate.h"
#include "coap-request-state.h"
#include "contiki.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_POST_PAYLOAD "Hello, COAP Cloud"

static struct etimer timer;
static coap_endpoint_t server_endpoint;
static coap_message_t request[1];

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 10);

  /* Define server endpoint */
  coap_endpoint_parse(COAP_SERVER_IP, sizeof(COAP_SERVER_IP) - 1, &server_endpoint);
  server_endpoint.port = COAP_SERVER_PORT;

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    etimer_reset(&timer);
    
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, COAP_URI_PATH);
    coap_set_payload(request, (uint8_t *)COAP_POST_PAYLOAD, sizeof(COAP_POST_PAYLOAD) - 1);
    
    coap_blocking_request_state_t request_state;
    COAP_BLOCKING_REQUEST(&server_endpoint, request,
      {
        printf("Response received\n");
        coap_show_pdu(COAP_LOG_INCOMING, (coap_message_t *)response);
      });
  }

  PROCESS_END();
}