#include <stdio.h>
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "msg.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

int main(void) {
    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t server_ep = { .family = AF_INET6,
                                .port = SERVER_PORT };

    ipv6_addr_from_str((ipv6_addr_t *)&server_ep.addr, SERVER_ADDR);
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    char payload[] = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, sizeof(payload));
    pdu.payload_len = sizeof(payload);

    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &server_ep, 
                                 NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request\n");
    } else {
        printf("Request sent\n");
    }
    
    return 0;
}