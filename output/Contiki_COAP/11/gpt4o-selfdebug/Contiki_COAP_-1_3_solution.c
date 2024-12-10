#include "net/ipv6/uip-nameserver.h"
#include "coap-request-state.h"
#include "coap-observe-client.h"
#include "coap-observe.h"
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

#include <stdio.h>
#include <string.h>

#define SERVER_EP "coap://[47.102.103.1]:5683"

/* Declare and define the CoAP server endpoint */
static const char *server_endpoint = SERVER_EP;

/*---------------------------------------------------------------------------*/
PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    static coap_message_t request[1];
    printf("Sending CoAP POST request to %s\n", server_endpoint);

    /* Prepare the request */
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    /* Send the CoAP request */
    COAP_BLOCKING_REQUEST(server_endpoint, request, response_handler);

    PROCESS_END();
}