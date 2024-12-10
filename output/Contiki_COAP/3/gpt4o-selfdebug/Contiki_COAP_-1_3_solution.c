#include "coap-request-state.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include <string.h>

#define COAP_BLOCKING_REQUEST_SYNC

PROCESS(hello_world_process, "Hello World Process");
AUTOSTART_PROCESSES(&hello_world_process);

static struct etimer et;

PROCESS_THREAD(hello_world_process, ev, data)
{
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];
  static char *uri_path = "/coap/test";

  PROCESS_BEGIN();
  
  // Configure endpoint
  coap_endpoint_parse("47.102.103.1", 5683, &server_ep);

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 10);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if(!coap_request_create(request, COAP_TYPE_CON, COAP_POST, 0, uri_path)) {
        printf("Could not create request\n");
        continue;
    }

    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request, NULL);
    printf("POST sent to %s%s\n", server_ep.addr_str, uri_path);
  }

  PROCESS_END();
}