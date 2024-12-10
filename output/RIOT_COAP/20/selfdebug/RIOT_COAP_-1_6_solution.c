#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define EMQX_URI_PATH                "/coap/test"
#define EMQX_SERVER_IP               "47.102.103.1"
#define COAP_METHOD_POST             (2)
#define CONFIG_GCOAP_PDU_BUF_SIZE    (128)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    
    if (pdu != NULL) {
        printf("Response received. Payload: %s\n", (char *)pdu->payload);
    }
    else {
        puts("No response received.");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    
    ssize_t result;
    int len;
    
    // Prepare the remote endpoint address
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result == 0) {
        printf("inet_pton: Invalid address\n");
        return 1;
    }
    else if (result == -1) {
        printf("inet_pton: Address conversion failed\n");
        return 1;
    }
    
    // Initialize the CoAP request
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;
    
    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    return 0;
}