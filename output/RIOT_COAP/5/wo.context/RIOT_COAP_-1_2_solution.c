#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/sock.h>
#include <stdio.h>
#include <string.h>

#define COAP_PORT      5683
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_URI_PATH  "/coap/test"
#define COAP_MESSAGE   "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_FAIL) {
        puts("Request failed");
        return;
    }

    if (pdu->hdr->code == COAP_CODE_205_CONTENT) {
        printf("Response: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    }
    else {
        puts("Invalid response");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);

    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = COAP_PORT
    };

    // Convert the string representation of the server IP into binary form
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP) == NULL) {
        puts("Failed to convert IP address");
        return 1;
    }

    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Failed to send request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}