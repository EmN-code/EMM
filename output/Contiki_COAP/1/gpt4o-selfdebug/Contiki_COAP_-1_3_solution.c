#include <stdio.h>
#include "coap-engine.h"
#include "os/dev/serial-line.h"
#include "os/sys/log.h"
#include "os/net/ipv6/uiplib.h"
#include <string.h>

#define LOG_MODULE "CoAP-Example"
#define LOG_LEVEL LOG_LEVEL_INFO

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];

PROCESS(coap_example_client, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client);

PROCESS_THREAD(coap_example_client, ev, data)
{
    PROCESS_BEGIN();

    LOG_INFO("CoAP Example Client Started\n");

    // Initialize the CoAP library
    coap_init_engine();

    // Configure the server IP address
    uip_ip6addr_t server_ipaddr;
    uiplib_ip6addrconv("47.102.103.1", &server_ipaddr);

    coap_endpoint_parse("coap://[47.102.103.1]:5683", strlen("coap://[47.102.103.1]:5683"), &server_endpoint);

    // Prepare the coap request
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");

    // Payload
    const char* payload = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)payload, strlen(payload));

    // Send the request
    LOG_INFO("Sending CoAP request\n");
    COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

    LOG_INFO("Request sent\n");

    PROCESS_END();
}