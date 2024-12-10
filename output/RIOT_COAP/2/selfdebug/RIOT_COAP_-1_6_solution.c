#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu) {
        printf("Response received: %s\n", (char *)pdu->payload);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t result;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result == 0) {
        printf("Invalid IPv4 address\n");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    const char *PAYLOAD = "Hello, COAP Cloud";
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    ssize_t len = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (len < 0) {
        printf("Failed to send request [%d]\n", (int)len);
        return 1;
    }

    printf("Request sent\n");
    return 0;
}
