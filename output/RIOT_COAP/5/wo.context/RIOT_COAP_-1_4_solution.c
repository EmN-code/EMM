#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define GCOAP_PORT 5683
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
const char *SERVER_IP = "47.102.103.1";
const char *EMQX_URI_PATH = "/coap/test";

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static sock_udp_ep_t remote;

gcoap_request_memo_t *memo;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response
}

int main() {
    ssize_t result;
    coap_pkt_t pdu;

    remote.port = GCOAP_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error parsing IP address\n");
        return -1;
    }

    memset(&pdu, 0, sizeof(coap_pkt_t));
    if (gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH) == NULL) {
        printf("Error initializing request\n");
        return -1;
    }

    gcoap_payload(&pdu, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    if (gcoap_req_send(&buf[0], pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending request\n");
        return -1;
    }

    printf("CoAP POST request sent\n");
    return 0;
}
