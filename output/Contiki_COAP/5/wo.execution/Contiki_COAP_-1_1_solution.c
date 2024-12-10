#include "coap-request-state.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"

#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer etimer;

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    etimer_set(&etimer, CLOCK_SECOND * 10);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etimer));

        coap_packet_t request[1];
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
        coap_set_header_uri_path(request, URI_PATH);
        const char *msg = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, strlen(msg));

        uip_ipaddr_t server_ipaddr;
        uiplib_ipaddrconv(SERVER_IP, &server_ipaddr);

        printf("Sending CoAP POST Request to %s%s\n", SERVER_IP, URI_PATH);
        COAP_BLOCKING_REQUEST(&server_ipaddr, SERVER_PORT, request);

        etimer_reset(&etimer);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&coap_client_process, NULL);
    return 0;
}