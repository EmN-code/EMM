#include "ztimer.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "msg.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

gcoap_resp_handler_t _resp_handler(coap_pkt_t *pdu, void *ctx)
{
    (void)ctx;
    printf("Response received with code %u\n", pdu->hdr->code);
    return GCOAP_NEXT;
}

int main(void)
{
    printf("Starting CoAP client\n");

    static sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    
    while (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error: Unable to convert server address\n");
        xtimer_sleep(1);
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    while (1) {
        res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Error: Failed to send request\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Request sent\n");
            break;
        }
    }
    return 0;
}