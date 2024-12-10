#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/mac/framer/framer.h"
#include "net/mac/mac.h"
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include "contiki-net.h"
#include "lib/random.h"
#include "sys/etimer.h"
#include "coap-engine.h"
#include "coap-request-state.h"
#include "mqtt-prop.h"
#include "mqtt.h"
#include "dht11-sensor.h"

/* COAP server endpoint details */
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

void response_handler(coap_message_t *response) {
    /* Assume we have a payload to handle the response */
    const uint8_t *payload = NULL;
    if(response) {
        payload = coap_get_payload(response, &len);
        printf("Response: %.*s\n", len, (char *)payload);
    }
}

PROCESS(coap_client, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client);

PROCESS_THREAD(coap_client, ev, data)
{
    PROCESS_BEGIN();

    static coap_endpoint_t server_ep;
    static coap_message_t request[1];  /* This way the packet can be treated as pointer as usual */

    coap_endpoint_parse(SERVER_IP, SERVER_PORT, &server_ep);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, URI_PATH);

    const char *msg = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, strlen(msg));

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    PROCESS_END();
}
