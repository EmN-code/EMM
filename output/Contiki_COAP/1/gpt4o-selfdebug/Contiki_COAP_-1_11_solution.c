#include "contiki.h"
#include "net/ipv6/uip-nameserver.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "coap-observe.h"
#include "coap-observe-client.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
    const uint8_t *payload = NULL;

    if (response == NULL) {
        printf("No response received\n");
        return;
    }

    int len = coap_get_payload(response, &payload);
    printf("Response received: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    printf("CoAP client process started\n");

    coap_endpoint_parse(COAP_SERVER_IP, COAP_SERVER_PORT, &server_endpoint);
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, COAP_URI_PATH);
    const char *msg = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, strlen(msg));

    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    PROCESS_END();
}