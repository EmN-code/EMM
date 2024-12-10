#include "contiki.h"
#include "coap-engine.h"
#include <stdio.h>

PROCESS(coap_client_process, "COAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

#define REMOTE_PORT     UIP_HTONS(5683)
#define REMOTE_ADDR     "[fdfd::1]"

static coap_endpoint_t server_ep;
static coap_message_t request[1];  /* This way the message can be treated as pointer as usual. */

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    /* Prepare the server endpoint address */
    coap_endpoint_parse(REMOTE_ADDR, REMOTE_PORT, &server_ep);

    while(1) {
        static struct etimer et;

        /* Wait for a period before sending the next request */
        etimer_set(&et, CLOCK_SECOND * 10);
        PROCESS_YIELD();

        if(etimer_expired(&et)) {
            /* Prepare the request message */
            coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
            coap_set_header_uri_path(request, "/coap/test");
            const char msg[] = "Hello, COAP Cloud";
            coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

            /* Send the request to the server */
            COAP_BLOCKING_REQUEST(&server_ep, request, NULL);
            printf("Request sent to %s\n", REMOTE_ADDR);
        }
    }

    PROCESS_END();
}