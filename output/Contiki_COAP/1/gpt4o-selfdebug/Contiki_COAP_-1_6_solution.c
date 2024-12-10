#include "sys/rtimer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "coap-timer.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"

PROCESS(coap_post_process, "CoAP POST Process");
AUTOSTART_PROCESSES(&coap_post_process);

static struct etimer timer;

PROCESS_THREAD(coap_post_process, ev, data)
{
    static coap_endpoint_t server_ep;
    static coap_message_t request[1]; /* Make space for the request */
    static coap_blocking_request_state_t request_state;

    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&timer)) {
            /* Prepare the message */
            coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
            coap_set_header_uri_path(request, "/coap/test");
            const char msg[] = "Hello, COAP Cloud";
            coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

            COAP_BLOCKING_REQUEST(&server_ep, request, coap_blocking_request_callback);

            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}

static void coap_blocking_request_callback(coap_message_t *response) 
{
    const uint8_t *appdata;
    int len = coap_get_payload(response, &appdata);
    printf("Response: %.*s\n", len, (char *)appdata);
}

#define SERVER_EP "coap://47.102.103.1:5683"