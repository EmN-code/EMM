#include "net/ipv6/uiplib.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6-route.h"

#include <contiki.h>
#include <contiki-net.h>
#include <coap-engine.h>
#include <coap-blocking-api.h>

/*---------------------------------------------------------------------------*/
PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);
/*---------------------------------------------------------------------------*/
static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request[1];

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND);

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    /* Wait until the timer expires. */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    /* Prepare the message */
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    /* Issue the blocking request */
    COAP_BLOCKING_REQUEST(&server_ep, request,
                          NULL);

    PROCESS_END();
}