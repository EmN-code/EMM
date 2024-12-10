#include <stdio.h>
#include <stdint.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT    CONFIG_GCOAP_PORT
#define URI_PATH       "/coap/test"

void coap_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_ADDRESS, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent < 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent\n");
    }
}

int main(void) {
    puts("RIOT CoAP Client Example");
    coap_request();
    return 0;
}