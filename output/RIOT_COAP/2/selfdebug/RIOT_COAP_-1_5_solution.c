#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/inet6.h>
#include <stdio.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_POST_URI_PATH "/coap/test"
#define COAP_METHOD_POST 2
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_POST_URI_PATH);
    static const char *PAYLOAD = "Hello, COAP Cloud";
    memcpy(&pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    res = gcoap_req_send(buf, pdu.hdr->payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Failed to send request\n");
        return 1;
    }
    printf("Request sent\n");

    return 0;
}
