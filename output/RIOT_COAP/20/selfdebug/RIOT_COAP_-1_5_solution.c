#include "net/gcoap.h"
#include "net/af.h"
#include "kernel_defines.h"
#include <stdio.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE (128)

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    remote.port = 5683;
    remote.family = AF_INET;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    
    ssize_t res;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    res = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res > 0) {
        printf("Successfully sent %d bytes\n", res);
    } else {
        printf("Error sending CoAP request\n");
    }

    return 0;
}