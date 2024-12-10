#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;
    unsigned code = coap_get_code(pdu);
    size_t payload_len;
    const uint8_t *payload = coap_get_payload(pdu, &payload_len);
    if (code == COAP_CODE_CHANGED) {
        printf("Success: %.*s\n", (int)payload_len, (const char *)payload);
    }
    else {
        printf("Error: CoAP request failed with code %u\n", code);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE,
                   COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);

    const char *data = "Hello, COAP Cloud";
    size_t data_len = strlen(data);
    memcpy(pdu.payload, data, data_len);

    gcoap_finish(&pdu, data_len, COAP_OPT_FINISH_PAYLOAD);

    res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote,
                         _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Failed to send request\n");
    }

    return 0;
}