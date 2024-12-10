#include <xtimer.h>
#include <stdio.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"
#define COAP_MSG_LEN (sizeof(COAP_MSG) - 1)

static ssize_t _build_post(uint8_t *buf, size_t len) {
    coap_pkt_t pdu;
    coap_hdr_set_type(buf, COAP_TYPE_CON);
    coap_hdr_set_code(buf, COAP_METHOD_POST);
    coap_hdr_set_id(buf, coap_get_id());
    coap_opt_add_uri(buf, URI_PATH);
    coap_payload(buf, (uint8_t *)COAP_MSG, COAP_MSG_LEN);
    pdu.payload_len = COAP_MSG_LEN;
    return (ssize_t)(coap_opt_finish(buf, COAP_OPT_FINISH_NONE) + pdu.payload_len);
}

static void _on_response(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                         const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("CoAP response received: ");
        printf("%.*s\n", pdu->payload_len, (char *)pdu->payload);
    }
    else {
        printf("No CoAP response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6);
    ssize_t len;
    int res;

    while (1) {
        len = _build_post(buf, sizeof(buf));
        res = gcoap_req_send(buf, len, &remote, _on_response, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Error sending CoAP request. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("CoAP request sent successfully!\n");
            break;
        }
    }

    while (1) {
        xtimer_sleep(10);
    }
    return 0;
}