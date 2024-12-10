#include <stdio.h>
#include "thread_config.h"
#include "net/nanocoap/cache.h"
#include "gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"
#define COAP_REQUEST_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response received: %u%% CoAP content\n", (unsigned)pdu->payload_len);
    printf("Payload: %.*s\n", pdu->payload_len, (char *)pdu->payload);
}

int main(void) {
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = COAP_SERVER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (len < 0) {
        puts("Error initializing CoAP request");
        return 1;
    }
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, COAP_REQUEST_PAYLOAD, strlen(COAP_REQUEST_PAYLOAD));
    len += strlen(COAP_REQUEST_PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}