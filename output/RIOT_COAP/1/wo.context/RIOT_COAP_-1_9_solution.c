#include <net/gcoap.h>
#include <net/af.h>
#include <net/netif.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define PAYLOAD             "Hello, COAP Cloud"
#define COAP_SERVER_IP      "47.102.103.1"
#define COAP_SERVER_PORT    5683
#define COAP_URI_PATH       "/coap/test"
#define BUF_SIZE            128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    puts("Response handler: message sent");
}

int main(void) {
    gcoap_listener_t listener;
    sock_udp_ep_t remote;
    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;

    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_SERVER_PORT;

    size_t len = gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res > 0) {
        printf("Successfully sent %d bytes\n", (int)res);
    } else {
        printf("Failed to send CoAP request\n");
    }

    return 0;
}
