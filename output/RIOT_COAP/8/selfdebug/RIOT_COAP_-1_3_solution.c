#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"
#define SOCKTYPE SOCK_ADDR_IP4

gcoap_socket_type_t type = GCOAP_SOCKET_TYPE_UDP;

static void response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("timeout");
        return;
    }
    if (pdu->code == COAP_CODE_EMPTY) {
        puts("gcoap: empty ACK");
    }
    else {
        printf("gcoap: response %u\n", coap_get_code_class(pdu->code));
    }
}

int main(void)
{
    msg_init_queue(msg, sizeof(msg)/sizeof(msg_t));
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    len += strlen(MESSAGE);

    if (gcoap_req_send(buf, len, &server, response_handler, NULL, type) < 0) {
        puts("Cannot send CoAP request");
    }

    return 0;
}