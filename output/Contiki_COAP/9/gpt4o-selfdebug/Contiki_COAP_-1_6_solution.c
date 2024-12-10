#include <contiki-net.h>
#include <contiki-lib.h>
#include <coap-request-state.h>

PROCESS(coap_post_process, "CoAP POST Request Process");
AUTOSTART_PROCESSES(&coap_post_process);

static struct etimer et;

void response_handler(coap_request_state_t *state) {
    printf("Response received from the server: status code: %d\n", state->response->code);
}

PROCESS_THREAD(coap_post_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 10);

    static coap_packet_t request[1];
    uip_ipaddr_t server_ipaddr;

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        uip_ip6addr(&server_ipaddr, 0x2f11, 0x6703, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001);

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
        coap_set_header_uri_path(request, "/coap/test");
        const char msg[] = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

        COAP_BLOCKING_REQUEST(&server_ipaddr, UIP_HTONS(5683), request, response_handler);

        printf("CoAP POST request sent to [47.102.103.1]:5683\n");
        etimer_reset(&et);
    }

    PROCESS_END();
}