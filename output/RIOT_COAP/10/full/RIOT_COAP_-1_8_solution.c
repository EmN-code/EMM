#include "ztimer.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "arpa/inet.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "netinet/in.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu) {
        printf("Response received %u bytes\n", pdu->payload_len);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = {.family = AF_INET, .port = SERVER_PORT};

    // Convert address
    while (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Address conversion failed. Retrying...\n");
        xtimer_sleep(1);
    }

    // Initialize the request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Set payload
    size_t payload_len = strlen(MESSAGE);
    memcpy(pdu.payload, MESSAGE, payload_len);
    coap_opt_add_payload_marker(&pdu);

    // Send CoAP request
    while (1) {
        ssize_t result = gcoap_req_send(buf, gcoap_get_request_length(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (result >= 0) {
            printf("Request sent successfully.\n");
            break;
        } else {
            printf("Failed to send request, retrying...\n");
            xtimer_sleep(1);
        }
    }

    return 0;
}
