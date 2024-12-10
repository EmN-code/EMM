#include "contiki.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "contiki-lib.h"
#include "coap-engine.h"
#include "net/ipv6/uip-nameserver.h"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static coap_endpoint_t server_ep;
static coap_message_t request[1]; /* This way the packet can be treated as pointer as usual. */

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();
    static coap_blocking_request_state_t request_state;

    /* Initialize the server endpoint */
    coap_endpoint_parse(SIMPLE_SMART_IP_ADDRESS, strlen(SIMPLE_SMART_IP_ADDRESS), &server_ep);

    /* Prepare the CoAP message */
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    /* Issue the blocking request */
    COAP_BLOCKING_REQUEST(&server_ep, request,
                          (void *)&request_state);

    PROCESS_END();
}

void coap_blocking_request_callback(void *callback_data,
                                     coap_message_t *response) {
    /* Handle the response which can include checking the response code, or extracting payload */
    const uint8_t *response_payload;
    int32_t response_payload_len = coap_get_payload(response, &response_payload);

    if (response_payload_len > 0) {
        printf("Received response: %.*s\n", (int)response_payload_len, response_payload);
    }
}