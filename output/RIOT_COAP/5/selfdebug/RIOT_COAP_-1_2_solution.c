#include <string.h>
#include <stdio.h>
#include <gcoap.h>

#define COAP_SERVER_IP      "47.102.103.1"
#define COAP_SERVER_PORT    CONFIG_GCOAP_PORT
#define URI_PATH            "/coap/test"

#define PAYLOAD             "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received: %s\n", (char *)pdu->payload);
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET6; // ensure correct family
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4); // correctly format IP
    remote.port = COAP_SERVER_PORT;

    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH) < 0) {
        printf("Error initializing coap request\n");
        return -1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    ssize_t result = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error sending coap request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}