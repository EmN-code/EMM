#include "coap-request-state.h"
#include "coap-observe.h"
#include "coap-observe-client.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

#include <stdio.h>

#define SERVER_EP "coap://47.102.103.1"
#define SERVER_PORT 5683
#define SERVER_URI "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    coap_endpoint_t server_endpoint;
    coap_message_t request[1];  // Use coap_message_t instead of unknown types

    coap_endpoint_parse(SERVER_EP, SERVER_PORT, &server_endpoint);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, SERVER_URI);

    const char *msg = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, strlen(msg));

    COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

    printf("Request sent successfully!\n");

    PROCESS_END();
}