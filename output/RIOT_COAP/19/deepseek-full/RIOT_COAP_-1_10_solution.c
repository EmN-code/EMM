
#include <stdio.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    // Placeholder for response handling logic
}

int main(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    local.port = 0;
    remote.port = EMQX_SERVER_PORT;
    remote.family = AF_INET;

    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid IP address");
        return 1;
    }

    result = sock_udp_create(&sock, &local, &remote, 0);
    if (result < 0) {
        puts("Failed to create socket");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    result = sock_udp_send(&sock, buf, len, &remote);
    if (result < 0) {
        puts("Failed to send CoAP request");
        sock_udp_close(&sock);
        return 1;
    }

    puts("CoAP request sent successfully");
    sock_udp_close(&sock);
    return 0;
}
