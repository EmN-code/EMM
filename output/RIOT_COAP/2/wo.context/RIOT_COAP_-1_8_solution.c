#include <string.h>
#include <stdio.h>
#include <net/inet/ipv4.h>          // Ensures inet_pton function
#include <net/gcoap.h>              // For CoAP functionalities

#define CONFIG_GCOAP_PDU_BUF_SIZE   (128)
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // to prevent unused warnings
    if (memo->state == GCOAP_MEMO_ERR) {
        printf("Error in response\n");
    } else if (memo->state == GCOAP_MEMO_RESP) {
        printf("Received response\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    int result;

    memset(&remote, 0, sizeof(remote));
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        puts("inet_pton failed");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending request: %d\n", (int)res);
        return -1;
    }

    puts("CoAP request sent\n");
    return 0;
}