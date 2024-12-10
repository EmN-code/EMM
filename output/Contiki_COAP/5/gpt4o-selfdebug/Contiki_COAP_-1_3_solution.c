#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "coap-engine.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    LOG_INFO("Starting CoAP client\n");

    // Initialize the CoAP endpoint structure
    coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);

    // Prepare the CoAP request message to POST "Hello, COAP Cloud"
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char *msg = "Hello, COAP Cloud";
    coap_set_payload(request, msg, strlen(msg));

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_endpoint, request,
        { LOG_INFO("Response received\n"); });

    LOG_INFO("Request sent successfully\n");

    PROCESS_END();
}

int main(void) {
    process_start(&coap_client_process, NULL);
    while (1) {
        process_run();
    }
    return 0;
}