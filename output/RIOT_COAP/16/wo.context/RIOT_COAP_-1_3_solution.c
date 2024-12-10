#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/nanocoap.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_IP          "47.102.103.1"
#define SERVER_PORT        5683
#define COAP_URI_PATH      "/coap/test"
#define COAP_MSG_CONTENT   "Hello, COAP Cloud"
#define COAP_BUF_SIZE      128

static gcoap_request_memo_t _memo;
static uint8_t _buf[COAP_BUF_SIZE];

static ssize_t _resp_handler(const gcoap_packet_t *pdu, const sock_udp_ep_t *remote, void *context) {
    (void) context;
    printf("Response from server: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    return 0;
}

int main(void) {
    sock_udp_ep_t remote = { .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, _buf, sizeof(_buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MSG_CONTENT, sizeof(COAP_MSG_CONTENT));
    len += sizeof(COAP_MSG_CONTENT);

    gcoap_req_send(_buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    return 0;
}