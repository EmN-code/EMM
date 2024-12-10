#include <stdio.h>
#include <stdlib.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include <string.h>

#define BUF_SIZE 128

static uint8_t buf[BUF_SIZE];
static gcoap_listener_t listener;

static void send_coap_request(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET6;  // Assuming IPv6
    remote.port = CONFIG_GCOAP_PORT;
    inet_pton(AF_INET6, "47.102.103.1", &remote.addr);

    size_t len = gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);  // Non-confirmable message
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    len += strlen("Hello, COAP Cloud");

    gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void) {
    puts("RIOT CoAP client example\n");
    send_coap_request();
    
    // Main loop
    while (1) {
        // Typically here your thread would wait for some event(s)
    }

    return 0;
}