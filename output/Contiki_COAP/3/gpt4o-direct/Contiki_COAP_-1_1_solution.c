#include <string.h>
#include <stdio.h>
#include "sys/log.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki.h"
#include "coap-engine.h"

//---------------------------------------------------------------------------
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SERVER_IP "[47.102.103.1]"
#define SERVER_PORT UIP_HTONS(5683)
#define COAP_URI_PATH "/coap/test"

process_event_t custom_event;

AUTOSTART_PROCESSES(&coap_client_process);

PROCESS(coap_client_process, "CoAP Client Process");

static void response_handler(coap_message_t *response) {
    const uint8_t *chunk;
    if(response == NULL) {
        LOG_INFO("Request timed out\n");
        return;
    }
    int len = coap_get_payload(response, &chunk);
    LOG_INFO("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_IP, SERVER_PORT, &server_ep);

    while(1) {
        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER || ev == custom_event) {
            LOG_INFO("Sending CoAP POST Request\n");
            coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
            coap_set_header_uri_path(request, COAP_URI_PATH);
            const char msg[] = "Hello, COAP Cloud";
            coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

            COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
        }
    }

    PROCESS_END();
}