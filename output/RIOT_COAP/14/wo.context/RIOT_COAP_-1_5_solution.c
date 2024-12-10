#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP_RECEIVED) {
        printf("Response received\n");
    }
    else {
        printf("Error receiving response\n");
    }
}

int main() {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t bytes;

    bytes = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (bytes < 0) {
        printf("Failed to initialize request\n");
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    size_t payload_length = strlen(PAYLOAD);
    gcoap_finish(&pdu, payload_length, NULL);

    ssize_t res = gcoap_req_send(buf, bytes, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Failed to send request\n");
    }

    return 0;
}