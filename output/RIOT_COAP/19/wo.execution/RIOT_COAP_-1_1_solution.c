#include <random.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote; // unused
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("No response received - server timed out");
        return;
    }

    printf("Response received: %u bytes, code: %1u.%02u\n",
           coap_get_total_hdr_len(pdu), (pdu->hdr->code >> 5), (pdu->hdr->code & 0x1F));
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ipv6_addr_t addr;
    sock_udp_ep_t remote = { .port = SERVER_PORT };

    if (ipv6_addr_from_str(&addr, SERVER_IP) == NULL) {
        puts("Error: unable to parse server IP address");
        return 1;
    }
    memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    coap_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    if (gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error: could not send CoAP request");
        return 1;
    }

    puts("CoAP request sent, waiting for response...");

    return 0;
}