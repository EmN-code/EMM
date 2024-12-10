#include "ztimer.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "arpa/inet.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define BUFFER_SIZE 128
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    ssize_t res;
    size_t len;

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    coap_pkt_t pdu;

    // Convert IPv4 address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) == 0) {
        printf("Invalid server IP address\n");
        return 1;
    }

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);

    // Set payload
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));

    // Set payload length
    if (!coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD)) {
        printf("Error finishing CoAP options\n");
        return 1;
    }
    len += strlen(payload);

    // Send CoAP request
    while (true) {
        res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res <= 0) {
            printf("Failed to send CoAP request: %d\n", (int)res);
            ztimer_sleep(ZTIMER_USEC, 1000000); // wait 1s before retrying
        } else {
            printf("CoAP request sent\n");
            break;
        }
    }

    return 0;
}