#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>

#define SERVER_IP            "47.102.103.1"
#define SERVER_PORT          CONFIG_GCOAP_PORT
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define URI_PATH             "/coap/test"
#define MSG_CONTENT          "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

gcoap_request_memo_t req_memo;
static sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response %u.%u from %s
",
           coap_get_code_class(pdu),
           coap_get_code_detail(pdu),
           SERVER_IP);
}

int main(void) {
    inet_pton(AF_INET6, SERVER_IP, &remote.addr);

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MSG_CONTENT, strlen(MSG_CONTENT));
    ssize_t len = gcoap_finish(&pdu, strlen(MSG_CONTENT), COAP_FORMAT_TEXT);

    if (len < 0) {
        printf("Error in gcoap_finish\n");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        // simulate activity
    }
    return 0;
}