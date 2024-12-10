#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/coap.h>
#include <arpa/inet.h>

#define COAP_METHOD_POST 0x02
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t res;

    // CoAP Packet Initialization
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Prepare the remote endpoint
    sock_udp_ep_t remote = { .port = 5683 };
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error converting server IP address\n");
        return -1;
    }

    // Send CoAP Request
    res = gcoap_req_send(buf, coap_hdr_len(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
        return -2;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Response timeout\n");
        return;
    }
    printf("Response received\n");
}