#include <stdio.h>
#include <stdlib.h>
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-udp-packet.h"
#include "coap-request-state.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static coap_endpoint_t server_endpoint;
static coap_message_t request;

void init_coap_endpoint(void) {
    uip_ipaddr_t server_ipaddr;
    uiplib_ipaddrconv(SERVER_IP, &server_ipaddr);
    server_endpoint.ipaddr = server_ipaddr;
    server_endpoint.port = SERVER_PORT;
}

void send_coap_post_request(void) {
    uint8_t buffer[128];
    uint16_t payload_length;

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", 17);

    coap_serialize_message(&request, buffer, &payload_length);
    coap_send_message(&server_endpoint, buffer, payload_length);
}

int main(void) {
    init_coap_endpoint();
    printf("Sending CoAP POST request to %s:%d\n", SERVER_IP, SERVER_PORT);
    send_coap_post_request();

    return 0;
}
