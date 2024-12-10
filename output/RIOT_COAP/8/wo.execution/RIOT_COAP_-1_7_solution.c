#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define COAP_METHOD COAP_METHOD_POST
#define BUF_SIZE 128

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, 
                          const sock_udp_ep_t *remote)
{
    if (req_state == GCOAP_MEMO_COMPLETE) {
        printf("Response received\n");
    }
    else {
        printf("Error in response\n");
    }
}

int main(void) {
    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    // Convert the string IP address to a binary format
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Malformed IP address\n");
        return 1;
    }

    // Initialize CoAP request
    int len = gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    
    return 0;
}