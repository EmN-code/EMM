#include <ztimer.h> // Ensure ztimer is included for timing
#include <xtimer.h> // Managing time for retries if needed
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/gcoap.h> // Include CoAP headers for gcoap features
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include "msg.h"
#include "net/sock/udp.h"

#define GCOAP_PORT   5683
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handling a response here (typically just logging)
    puts("Response received from server");
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    sock_udp_ep_t remote = {.family = AF_INET, .port = GCOAP_PORT};
    while (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("IP address conversion failed, retrying...\n");
        xtimer_sleep(1);
    }

    while (1) {
        int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
        if (len < 0) {
            printf("Failed to initialize CoAP request\n");
            continue;
        }

        if (!coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE)) {
            printf("Failed to finish CoAP options\n");
            continue;
        }

        char *message = "Hello, COAP Cloud";
        memcpy(pdu.payload, message, strlen(message));
        len += strlen(message);

        // Retry send in case of failure
        while (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
            printf("Failed to send CoAP request, retrying...\n");
            xtimer_sleep(1);
        }

        // Successfully sent
        printf("CoAP request sent successfully\n");
        xtimer_sleep(10);  // Sleep before sending the next request
    }

    return 0;
}