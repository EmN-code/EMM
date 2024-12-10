#include "string.h"
#include "stdlib.h"
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "net/routing/routing.h"
#include "coap-endpoint.h"
#include "coap-request-state.h"
#include "coap-timer.h"
#include "coap-transport.h"
#include "coap-observe-client.h"
#include "net/netstack.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/simple-udp.h"
#include "net/mac/framer/framer.h"
#include "sys/etimer.h"
#include "lib/random.h"
#include "udp-socket.h"
#include "tcp-socket.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;

static void send_coap_request(){
    uip_ipaddr_t server_ip;
    uiplib_ipaddrconv(COAP_SERVER_IP, &server_ip);
    
    coap_endpoint_t endpoint;
    coap_endpoint_init(&endpoint, &server_ip, UIP_HTONS(COAP_SERVER_PORT));

    static uint8_t request_buffer[128];
    size_t payload_len = sizeof(COAP_PAYLOAD) - 1;
    static uint8_t token[] = {0x00};

    coap_transaction_t *transaction;
    transaction = coap_new_transaction(coap_get_mid(), &endpoint);
    if(transaction) {
        coap_packet_t *request = (coap_packet_t *)transaction->message;
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
        coap_set_token(request, token, sizeof(token));
        coap_set_header_uri_path(request, COAP_URI_PATH);
        
        memcpy(request_buffer, COAP_PAYLOAD, payload_len);
        coap_set_payload(request, request_buffer, payload_len);

        coap_send_transaction(transaction);
    }
}

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();
    
    etimer_set(&et, CLOCK_SECOND * 10);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        send_coap_request();

        etimer_reset(&et);
    }

    PROCESS_END();
}