#include "xtimer.h"
#include "thread.h"
#include "net/gcoap.h"

#define GCOAP_PORT 5683
#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote;

int main(void) {
    printf("Starting CoAP client example...\n");

    /* Initialize the remote address struct */
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_ADDR);
    remote.port = GCOAP_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    while (1) {
        /* Initialize request message */
        gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        size_t payloadlen = strlen(COAP_PAYLOAD);
        coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
        memcpy(pdu.payload, COAP_PAYLOAD, payloadlen);
        gcoap_finish(&pdu, payloadlen, COAP_OPT_FINISH_PAYLOAD);

        int res = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Error sending request, retrying...\n");
            xtimer_sleep(1);
            continue;
        }

        printf("CoAP request sent successfully\n");
        break;
    }

    return 0;
}