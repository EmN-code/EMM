
#include "contiki.h"
#include "coap.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static struct coap_endpoint_t server_endpoint;
static struct coap_message_t request;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
    static struct etimer et;

    PROCESS_BEGIN();

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server_endpoint, COAP_SERVER_IP, COAP_SERVER_PORT);

    // Create a CoAP POST request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Send the CoAP request
    coap_blocking_send(&server_endpoint, &request);

    // Timer to periodically send the request
    etimer_set(&et, CLOCK_SECOND * 5);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        etimer_reset(&et);
        coap_blocking_send(&server_endpoint, &request);
    }

    PROCESS_END();
}

int main(void)
{
    process_start(&coap_client_process, NULL);
    return 0;
}
