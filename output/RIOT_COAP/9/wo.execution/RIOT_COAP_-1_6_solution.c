#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "arpa/inet.h"

#define EMQX_SERVER_IP      "47.102.103.1"
#define EMQX_URI_PATH      "/coap/test"
#define PAYLOAD            "Hello, COAP Cloud"

// Avoid defining CONFIG_GCOAP_PDU_BUF_SIZE here to fix redefinition error

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    puts("Response received!");
}

int main(void) {
    printf("Starting CoAP Client\n");

    static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t len;
    coap_pkt_t pdu;

    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = 5683;

    // Convert IP string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error converting IP address\n");
        return 1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending request\n");
        return 1;
    }

    return 0;
}