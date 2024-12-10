#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <string.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote){
    /*Response handler code here*/
}

int main() {
    sock_udp_ep_t remote = { .port = EMQX_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    
    // Convert the IP address from text to binary form
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: Unable to convert IP address\n");
        return -1;
    }

    // Initialize CoAP request
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    
    // Send CoAP request
    ssize_t bytes_sent = gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes_sent < 0) {
        printf("Error: Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}