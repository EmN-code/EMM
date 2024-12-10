#include <net/gcoap.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == 0) {
        printf("Request timed out\n");
    } else if (pdu && coap_get_code(pdu)) {
        printf("Response %u.%u\n", coap_get_code(pdu) >> 5, coap_get_code(pdu) & 0x1F);
    } else {
        printf("Response error\n");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    ssize_t bytes_sent;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    int result;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting server IP");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    bytes_sent = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error sending packet\n");
    }

    return 0;
}