// This program sends a CoAP POST request using RIOT OS on ESP32 to a CoAP server.

#include "ztimer.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "string.h"
#include "msg.h"
#include "mutex.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    if (pdu != NULL) {
        printf("Received response, code: %u\n", coap_pkt_get_code(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    // Convert IPv4 address from string representation to binary
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Malformed IPv4 address\n");
        return -1;
    }

    while (1) {
        int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        if (res < 0) {
            printf("Error initializing request\n");
            return -1;
        }

        const char payload[] = "Hello, COAP Cloud";
        ssize_t bytes = coap_payload(&pdu, (uint8_t *)payload, strlen(payload));
        if (bytes < 0) {
            printf("Error setting payload\n");
            return -1;
        }

        if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
            printf("Failed to send request\n");
            ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second before retrying
            continue;
        }
        printf("Request sent successfully\n");
        break;
    }

    return 0;
}