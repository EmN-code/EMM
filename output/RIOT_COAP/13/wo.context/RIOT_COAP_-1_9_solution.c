#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_METHOD_POST 2

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Placeholder for response handling logic
    if (pdu != NULL) {
        printf("Response received\n");
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    size_t len;

    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination address\n");
        return 1;
    }

    len = gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len == 0) {
        printf("Error initializing request\n");
        return 1;
    }

    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    gcoap_hdr_set_type(&pdu, COAP_TYPE_CON);
    len += strlen("Hello, COAP Cloud");

    if (gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) == 0) {
        printf("Error sending request\n");
        return 1;
    }

    printf("Request sent\n");
    return 0;
}