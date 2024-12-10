#include "coap.h"
#include "coap-request-state.h"
#include "coap-observe.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"

#define SERVER_EP      "coap://47.102.103.1"
#define SERVER_PORT    5683
#define COAP_PATH      "/coap/test"

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

static coap_endpoint_t server_endpoint;
static coap_blocking_request_state_t request_state;
static char payload[] = "Hello, COAP Cloud";

void response_handler(coap_message_t *response) {
    printf("Response received: %d\n", coap_get_status_code(response));
}

PROCESS_THREAD(coap_client_example, ev, data) {
    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_EP, SERVER_PORT, &server_endpoint);

    static coap_message_t request[1];
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, COAP_PATH);
    coap_set_payload(request, (uint8_t *)payload, sizeof(payload) - 1);

    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    printf("CoAP request sent\n");

    PROCESS_END();
}