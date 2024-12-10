#include "uiplib.h"
#include "uip-udp-packet.h"
#include "uip-nameserver.h"
#include "uip-ds6-route.h"
#include "uip-ds6-nbr.h"
#include "uip-ds6.h"
#include "tcp-socket.h"
#include "process.h"
#include "net/ip/uip.h"
#include "net/ip/uip-debug.h"
#include "net/ip/uip-udp-packet.h"
#include "dev/leds.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "contiki.h"
#include <string.h>

/* Define the IP address and port of the CoAP server */
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT UIP_HTONS(5683)
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
    const uint8_t *payload = NULL;
    if (response != NULL) {
        coap_get_payload(response, &payload);
        printf("Response: %s\n", payload);
    }
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];  // This way the packet can be treated as pointer as usual

    PROCESS_BEGIN();

    /* Initialize the server address */
    uip_ipaddr_t server_ipaddr;
    uiplib_ipaddrconv(SERVER_IP, &server_ipaddr);

    /* Configure the CoAP server endpoint */
    coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
    server_ep.port = SERVER_PORT;

    /* Initialize CoAP engine */
    coap_init_engine();

    while (1) {
        /* Prepare the CoAP request */
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, URI_PATH);
        const char msg[] = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

        /* Issue the request */
        COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

        /* Wait for some time before next request */
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    }

    PROCESS_END();
}