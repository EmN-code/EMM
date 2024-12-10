#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "arpa/inet.h"
#include "netinet/in.h"

#define COAP_METHOD_POST  2
#define COAP_URI_PATH     "/coap/test"
#define COAP_PAYLOAD      "Hello, COAP Cloud"
#define SERVER_IP         "47.102.103.1"
#define SERVER_PORT       5683

static void _response_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (req_state == GCOAP_MEMO_VALIDATE) {
        printf("Response received: %u\n", coap_get_code_raw(pdu));
    }
    else {
        printf("Error receiving response\n");
    }
}


int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&pdu, 0, sizeof(pdu));
    memset(&remote, 0, sizeof(remote));

    remote.family = AF_INET;
    remote.netif  = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP request
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    // Set payload for the request
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
    }

    return 0;
}