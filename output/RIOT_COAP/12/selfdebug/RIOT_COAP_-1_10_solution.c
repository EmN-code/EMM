#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>  // Required for inet_pton
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Define only once to avoid redefinition error

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response here
}

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int res;
    const char *PAYLOAD = "Hello, COAP Cloud";

    // Initialize remote endpoint
    remote.family = AF_INET;
    remote.port = 5683;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    if (res < 0) {
        printf("gcoap_req_init failed: %d\n", res);
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("gcoap_req_send failed: %d\n", res);
        return 1;
    }

    printf("Request sent successfully.\n");
    return 0;
}