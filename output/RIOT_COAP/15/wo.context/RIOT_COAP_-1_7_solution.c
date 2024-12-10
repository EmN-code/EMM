#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/gnrc/netif.h>
#include <arpa/inet.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static const char *coap_data = "Hello, COAP Cloud";
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    int res;

    while (1) {
        res = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (res <= 0) {
            printf("inet_pton failed\n");
            xtimer_sleep(1);
            continue;
        }
        printf("inet_pton success\n");
        break;
    }

    res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (res < 0) {
        printf("Failed to init CoAP request\n");
        return 1;
    }

    size_t len = coap_put_block2(&pdu, coap_data, strlen(coap_data), 0, 0, false);
    if (len == 0) {
        printf("Failed to format CoAP request\n");
        return 1;
    }

    while (1) {
        res = gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Failed to send CoAP request\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Sent CoAP request\n");
        break;
    }

    return 0;
}