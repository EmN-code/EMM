#include "random.h"
#include "mqtt-prop.h"
#include "coap-log.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "contiki-net.h"
#include "contiki.h"
#include "contiki-conf.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include "coap-engine.h"
#include "mqtt.h"
#include "coap-blocking-api.h"
#include "coap-endpoint.h"
#include "net/ipv6/uip-icmp6.h"
#include "dev/button-hal.h"
#include "coap-timer.h"
#include "udp-socket.h"
#include "contiki-lib.h"
#include "#include coap-request-state.h"
#include "#include coap-transactions.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-nameserver.h"
#include "string.h"
#include "sys/etimer.h"
#include "net/mac/framer/framer.h"
#include "#include coap-observe-client.h"
#include "#include coap-separate.h"
#include "tcp-socket.h"
#include "coap-observe.h"
#include "coap-transport.h"
#include "contiki-default-conf.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define POST_INTERVAL (60 * CLOCK_SECOND)

static struct etimer et;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    static coap_endpoint_t server_ep;
    static coap_message_t request[1];  // This way the packet can be treated as pointer as usual

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);
    etimer_set(&et, POST_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&et)) {
            printf("Sending CoAP POST request to %s\n", SERVER_EP);

            coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
            coap_set_header_uri_path(request, "/coap/test");

            const char msg[] = "Hello, COAP Cloud";
            coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

            COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

            etimer_reset(&et);
        }
    }

    PROCESS_END();
}