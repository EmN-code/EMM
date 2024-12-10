#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include "net/sock/udp.h" 
#include "net/gcoap.h" 

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    coap_pkt_t pdu;
    ssize_t bytes;

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    len += 17;

    // Send CoAP request
    bytes = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP POST request sent\n");
    return 0;
}