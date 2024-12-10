#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include "dev/etc/rgb-led/rgb-led.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define POST_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer timer;

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 10);

    uip_ipaddr_t server_ipaddr;
    uip_ip6addr(&server_ipaddr, 0x2f7, 0x5766, 0xa487, 0x0, 0x02, 0x577, 0x01, 0x2);

    static coap_endpoint_t server_endpoint;

    coap_endpoint_parse(SERVER_IP, SERVER_PORT, &server_endpoint);
    static coap_message_t request[1];

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, POST_URI_PATH);

        const char *msg = "Hello, COAP Cloud";
        unsigned int len = strlen(msg);
        coap_set_payload(request, (uint8_t *)msg, len);

        COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

        etimer_reset(&timer);
    }

    PROCESS_END();
}