#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/coap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COAP_SERVER "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static struct uip_udp_conn *server_conn;

void send_coap_request(void) {
    uip_ipaddr_t server_ipaddr;
    uiplib_ipaddrconv(COAP_SERVER, &server_ipaddr);

    server_conn = udp_new(&server_ipaddr, UIP_HTONS(COAP_PORT), NULL);
    if(server_conn == NULL) {
        printf("Failed to create UDP connection\n");
        return;
    }

    static uint8_t coap_req[128]; // adjust buffer size as needed
    uint32_t block_num = 0;
    
    coap_packet_t request[1];      // CoAP packet 
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, COAP_URI_PATH);
    coap_set_payload(request, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    COAP_BLOCKING_REQUEST(server_conn, request, NULL);
}

int main(void) {
    printf("Starting CoAP client\n");

    while(1) {
        send_coap_request();
        PROCESS_WAIT_EVENT();
    }

    return 0;
}
