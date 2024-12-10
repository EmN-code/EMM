#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#include "net/gcoap.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    size_t payload_len;
    const uint8_t *payload = coap_get_payload(pdu, &payload_len);
    printf("Response received - Code: %u Payload: %.*s\n", coap_get_code(pdu), (int)payload_len, payload);
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int res;

    remote.port = COAP_SERVER_PORT;
    res = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    const char *message = "Hello, COAP Cloud";
    size_t message_len = strlen(message);
    memcpy(gcoap_pdu_buf(&pdu) + gcoap_get_offset(&pdu), message, message_len);
    gcoap_set_payload(&pdu, message, message_len);

    ssize_t sent_bytes = gcoap_req_send(buf, gcoap_get_len(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (sent_bytes <= 0) {
        printf("Error sending request: %d\n", (int)sent_bytes);
    }

    return 0;
}