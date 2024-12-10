#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>

#include "gcoap.h"

#define PAYLOAD "Hello, COAP Cloud"
#define EMQX_URI_PATH "/coap/test"

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t result;
    const char *server_ip = "47.102.103.1";

    // Convert IP address string to binary form
    result = inet_pton(AF_INET, server_ip, &remote.addr);
    if (result <= 0) {
        printf("Error in IP conversion\n");
        return -1;
    }

    // Initialize CoAP request
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (result < 0) {
        printf("Error initializing CoAP request\n");
        return -1;
    }

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send request
    gcoap_req_send(buf, pdu.hdr_len + strlen(PAYLOAD), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;

    printf("CoAP response received\n");
}