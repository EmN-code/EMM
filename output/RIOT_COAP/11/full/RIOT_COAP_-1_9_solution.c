#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

// Callback function for handling the response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        char *response = (char *)pdu->payload;
        printf("Response received: %s\n", response);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    char EMQX_SERVER_IP[] = "47.102.103.1";
    char EMQX_URI_PATH[] = "/coap/test";
    char PAYLOAD[] = "Hello, COAP Cloud";

    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Resolve the IP address
    while (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address. Retrying in 1 second...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    ssize_t res;
    while (1) {
        res = gcoap_req_send(&buf[0], strlen(PAYLOAD), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res > 0) {
            printf("Request sent successfully\n");
            break;
        } else {
            printf("Failed to send request. Retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }
    }

    return 0;
}