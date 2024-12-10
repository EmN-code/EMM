#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define POST_MESSAGE "Hello, COAP Cloud"
#define COAP_BUF_SIZE 128

static ssize_t _resp_handler(coap_pkt_t* pkt, const sock_udp_ep_t *remote, void *context)
{
    (void)context;  // Unused parameter
    printf("Response received from %s\n", SERVER_ADDR);
    return 0;
}

int main(void)
{
    uint8_t buf[COAP_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse IPv4 address\n");
        return -1;
    }

    while (gcoap_req_init(&pdu, buf, COAP_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH) == -1) {
        printf("Error: failed to initialize CoAP request. Retrying...\n");
        xtimer_sleep(1);
    }

    coap_hdr_set_type(&pdu, COAP_TYPE_NON);
    size_t len = gcoap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, POST_MESSAGE, sizeof(POST_MESSAGE));
    len += sizeof(POST_MESSAGE)-1; 

    while (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) == -1) {
        printf("Error: failed to send CoAP request. Retrying...\n");
        xtimer_sleep(1);
    }

    printf("POST request sent to the CoAP server.\n");

    return 0;
}