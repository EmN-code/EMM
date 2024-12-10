#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/gnrc.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 2

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    const char *payload = "Hello, COAP Cloud";
    const char *server_ip = "47.102.103.1";
    const char *uri_path = "/coap/test";
    int res;

    /* Prepare CoAP request */
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, uri_path);
    if (len <= 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    /* Configure remote address */
    remote.family = AF_INET;
    res = inet_pton(AF_INET, server_ip, &remote.addr.ipv4);
    if (res <= 0) {
        printf("Error converting server IP\n");
        return 1;
    }
    remote.port = 5683;

    /* Send the CoAP request */
    res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}