#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_APP

#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/simple-udp.h"
#include "net/netstack.h"
#include "net/mac/framer/framer.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "dev/button-hal.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "stdlib.h"
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "coap-engine.h"
#include "coap-request-state.h"
#include "coap-blocking-api.h"
#include "coap-endpoint.h"
#include "coap-observe-client.h"
#include "coap-transport.h"
#include "coap-timer.h"
#include "sys/log.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "random.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"
char *service_urls[] = {"/coap/test", NULL};
 
PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request[1];  // This way the packet can be treated as pointer as usual

PROCESS_THREAD(coap_client_example, ev, data)
{
    PROCESS_BEGIN();

    static coap_message_t response;

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    etimer_set(&et, CLOCK_SECOND * 5);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        etimer_reset(&et);

        /* prepare request, TID is set by COAP_BLOCKING_REQUEST() */
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, service_urls[0]);
        const char msg[] = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

        LOG_INFO("Sending %s to %s\n", msg, SERVER_EP);

        COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    }

    PROCESS_END();
}

static void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);

  LOG_INFO("Response: %.*s\n", len, (char *)chunk);
}