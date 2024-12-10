#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>
#include <string.h>
#include <stdio.h>

#define EMQX_URI_PATH "/coap/test"
#define SERVER_IP "47.102.103.1"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received, payload: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    ssize_t len;

    // Initialize the CoAP PDU buffer
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        puts("Error initializing CoAP request");
        return -1;
    }

    // Add payload to PDU
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    // Set remote endpoint address
    if (inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6) != 1) {
        puts("Error converting server IP");
        return -1;
    }
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending CoAP request");
        return -1;
    }

    puts("CoAP request sent successfully");
    return 0;
}