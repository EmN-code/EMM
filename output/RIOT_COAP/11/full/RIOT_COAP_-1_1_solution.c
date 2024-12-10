#include <stdio.h>
#include <unistd.h>
#include "gcoap.h"
#include "net/af.h"
#include "net/conn/ip.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;

    if (pdu->code == COAP_CODE_205_CONTENT) {
        printf("Response received: %s\n", (char *)pdu->payload);
    } else {
        printf("Failed to receive a valid response\n");
    }
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
    };

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP) == NULL) {
        printf("Invalid IP address\n");
        return 1;
    }
    remote.port = COAP_SERVER_PORT;

    while (1) {
        len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
        coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
        memcpy(pdu.payload, "Hello, COAP Cloud", 18);
        len += 18;

        if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL) < 0) {
            printf("Failed to send CoAP request\n");
            xtimer_sleep(1);
            continue;
        }
        printf("CoAP POST request sent\n");
        break;
    }

    return 0;
}