#include "net/ipv6/udp-socket.h"
#include "contiki-net.h"
#include "contiki-conf.h"
#include "coap-endpoint.h"
#include "coap-blocking-api.h"
#include "lib/random.h"
#include "net/mac/framer/framer.h"
#include "contiki.h"
#include "net/ipv6/uip-icmp6.h"
#include "stdlib.h"
#include "dev/button-hal.h"
#include "coap-timer.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uiplib.h"
#include "contiki-lib.h"
#include "net/ipv6/tcp-socket.h"
#include "coap-request-state.h"
#include "net/ipv6/uip-nameserver.h"
#include "string.h"
#include "sys/etimer.h"
#include "net/routing/routing.h"
#include "coap-engine.h"
#include "tcp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "mqtt-prop.h"
#include "coap-observe-client.h"
#include "coap-log.h"

#define REMOTE_PORT     UIP_HTONS(5683)
static uip_ipaddr_t server_ipaddr;

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

static void response_handler(coap_message_t *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    printf("Received response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_example, ev, data)
{
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    uiplib_ipaddrconv("47.102.103.1", &server_ipaddr);
    coap_endpoint_parse(SERVER_IP_ADDR, REMOTE_PORT, &server_ep);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    PROCESS_END();
}