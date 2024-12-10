#include "net/ipv6/udp-socket.h"
#include "net/mac/framer/framer.h"
#include "stdlib.h"
#include "dev/button-hal.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "sys/etimer.h"
#include "mqtt-prop.h"
#include "dht11-sensor.h"
#include "net/netstack.h"
#include "random.h"
#include "coap-log.h"
#include "coap-request-state.h"
#include "coap-engine.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define TOGGLE_INTERVAL (5 * CLOCK_SECOND)

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

static struct etimer et;

PROCESS_THREAD(coap_client_example, ev, data)
{
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    etimer_set(&et, TOGGLE_INTERVAL);

    while(1) {

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, "/coap/test");
        const char msg[] = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

        COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}