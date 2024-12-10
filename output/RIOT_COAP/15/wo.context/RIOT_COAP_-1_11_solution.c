#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static const char *COAP_SERVER_IP = "47.102.103.1";
static const char *COAP_URI_PATH = "/coap/test";

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // response handler implementation
}

int main(void) {
    int rc;
    printf("Configuring remote endpoint\n");

    // Convert IP address
    while (true) {
        rc = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (rc <= 0) {
            printf("Error converting IP address. Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        break;
    }

    coap_pkt_t pdu;
    rc = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (rc < 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    // Add payload "Hello, COAP Cloud"
    char payload[] = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send request
    while (true) {
        rc = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc < 0) {
            printf("Failed to send CoAP request. Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("CoAP request sent successfully\n");
        break;
    }

    return 0;
}