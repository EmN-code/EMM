#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "contiki-lib.h"
#include "contiki.h"

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer timer;

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  /* Configure the event timer */
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    /* Wait for the periodic timer to expire and then perform CoAP POST */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    /* Re-arm the timer to continue the periodic operation */
    etimer_reset(&timer);

    /* Code to send a CoAP POST request */
    static coap_message_t request[1];
    static coap_endpoint_t remote_endpoint;
    coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &remote_endpoint);
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    
    const char *msg = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&remote_endpoint, request,
                          NULL);
  }

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  while(1) {
    // Run the process scheduler to handle events
    process_run();
  }
  return 0;
}