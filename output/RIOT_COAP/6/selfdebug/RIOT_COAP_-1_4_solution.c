#include <string.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <net/sock/dtls.h>
#include <net/ipv6/addr.h>
#include <net/nanocoap/cache.h>
#include <kernel_defines.h>
#include <hashes/sha2xx_common.h>
#include <stdio.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE      (128)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);
int gcoap_req_send(const uint8_t *buf, size_t len, const sock_udp_ep_t *remote, gcoap_resp_handler_t resp_handler, void *context, gcoap_socket_type_t tl_type);
int gcoap_req_init(coap_pkt_t *pdu, uint8_t *buf, size_t len, unsigned code, const char *path);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    coap_pkt_t pdu;

    const char *EMQX_URI_PATH = "/coap/test";
    const char *COAP_SERVER_IP = "47.102.103.1";
    sock_udp_ep_t remote = { .family = AF_INET, .port = CONFIG_GCOAP_PORT };

    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    const char *PAYLOAD = "Hello, COAP Cloud";

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    len += strlen(PAYLOAD);

    gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;

    printf("Response received\n");
}