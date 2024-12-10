#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include <stdio.h>
#include <string.h>

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote){
    if (memo->state != GCOAP_MEMO_DONE) {
        puts("Error in handling response");
    } else {
        printf("Received response with code %u\n", coap_get_code(pdu));
    }
}

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_ADDR, &server.addr);

    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    
    ssize_t res = gcoap_req_send(buf, len, &server, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending request: %d\n", (int)res);
    }

    return 0;
}