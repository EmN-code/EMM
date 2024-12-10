#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"

#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     5683
#define URI_PATH        "/coap/test"
#define PAYLOAD         "Hello, COAP Cloud"
#define BUF_SIZE        CONFIG_GCOAP_PDU_BUF_SIZE

static uint8_t buf[BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("timeout");
        return;
    }

    unsigned code = coap_get_code(pdu);
    printf("Response Code: %u\n", code);
    if (code != COAP_CODE_EMPTY) {
        char content[64];
        size_t payload_len = coap_get_payload(pdu, (uint8_t **)content);
        printf("Content: %.*s\n", (int)payload_len, content);
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };

    // Convert text IP address to binary form
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_IP) == NULL) {
        puts("Error: Invalid server IP address");
        return 1;
    }

    size_t len = gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending request");
        return 1;
    }

    return 0;
}