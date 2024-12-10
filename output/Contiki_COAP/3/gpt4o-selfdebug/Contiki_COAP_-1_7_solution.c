#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki.h"
#include "coap-request-state.h"

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "/coap/test"

PROCESS(coap_example_client_process, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client_process);

static void response_handler(coap_message_t *response) {
    const uint8_t *payload = NULL;
    int len = coap_get_payload(response, &payload);
    printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_example_client_process, ev, data) {
    static coap_message_t request[1];
    static struct etimer et;
    coap_endpoint_t server_ep;
    static coap_blocking_request_state_t request_state;

    PROCESS_BEGIN();

    uip_ipaddr_t server_ip;
    uip_ip6addr(&server_ip, 0x47, 0x66, 0x6a, 1);
    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    while (1) {
        etimer_set(&et, CLOCK_SECOND * 10);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, URI_PATH);
        const char msg[] = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

        COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
    }

    PROCESS_END();
}

int main(void) {
    return 0;
}