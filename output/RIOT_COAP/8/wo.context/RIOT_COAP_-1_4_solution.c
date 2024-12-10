#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Ensure this is defined only once

int main() {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Convert the string IP to a binary format
    const char *EMQX_SERVER_IP = "47.102.103.1";
    int result;
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        fprintf(stderr, "inet_pton failed\n");
        return -1;
    }

    // Set the CoAP server endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    const char *PAYLOAD = "Hello, COAP Cloud";
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        fprintf(stderr, "Failed to send CoAP request\n");
        return -1;
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Received response\n");
}