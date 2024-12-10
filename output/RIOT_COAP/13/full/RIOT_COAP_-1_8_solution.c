#include <xtimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <net/ipv6/addr.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"
#define MAX_RETRY 5

int main(void) {
    // Prepare destination endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error parsing IP address\n");
        return -1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int retry_count = 0;

    // Initialize CoAP request
    while (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH) == NULL) {
        printf("Failed to initialize CoAP request\n");
        if (++retry_count >= MAX_RETRY) {
            return -1;
        }
        xtimer_sleep(1);
    }

    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);

    // Set payload
    if (coap_payload(&pdu, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD)) == NULL) {
        printf("Failed to set CoAP payload\n");
        return -1;
    }

    ssize_t res;
    while ((res = gcoap_req_send(buf, pdu.hdr->len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP)) <= 0) {
        if (res < 0) {
            printf("Error sending CoAP request: %ld\n", res);
        }
        if (++retry_count >= MAX_RETRY) {
            return -1;
        }
        xtimer_sleep(1);
    }

    printf("CoAP request sent\n");
    return 0;
}

// Response handler
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (!memo->state) {
        printf("No response received\n");
        return;
    }
    printf("Response received\n");
}