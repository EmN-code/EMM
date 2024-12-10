#include <xtimer.h>
#include <ztimer.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <msg.h>

#define COAP_PORT           5683
#define COAP_BUF_SIZE       CONFIG_GCOAP_PDU_BUF_SIZE
#define COAP_SERVER_ADDR    "47.102.103.1"
#define COAP_URI_PATH       "/coap/test"
#define COAP_PAYLOAD        "Hello, COAP Cloud"

gcoap_resp_handler_t _resp_handler;
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    /* Handle the response or error */
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    char buf[COAP_BUF_SIZE];
    coap_pkt_t pdu;
    int res;

    puts("RIOT CoAP example");

    /* Initialize destination address */
    res = inet_pton(AF_INET, COAP_SERVER_ADDR, &remote.addr);
    if (res == 0) {
        puts("ERROR: Unable to parse destination address");
        return 1;
    }

    /* Initialize CoAP packet */
    gcoap_req_init(&pdu, (uint8_t *)buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    /* Finish the CoAP options and set payload length */
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    /* Send the CoAP request */
    if (gcoap_req_send((uint8_t *)buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("ERROR: sending CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}