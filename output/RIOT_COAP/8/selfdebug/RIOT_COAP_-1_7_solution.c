#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>

// Redefine macros appropriately
#undef COAP_PORT
#define COAP_PORT 5683

#undef CONFIG_GCOAP_PDU_BUF_SIZE
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

#define SERVER_IP "47.102.103.1"
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo,
                          coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response: %u\n", coap_get_code(pdu));
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    memset(buf, 0, CONFIG_GCOAP_PDU_BUF_SIZE);
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Correctly using inet_pton without issues
    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        if (result == 0) {
            printf("inet_pton: Not in presentation format\n");
        } else {
            perror("inet_pton");
        }
        return 1;
    }
    remote.family = AF_INET;
    remote.port = COAP_PORT;

    gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len + pdu.opt_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}