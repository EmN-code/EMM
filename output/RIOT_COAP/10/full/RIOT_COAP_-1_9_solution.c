#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdint.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "string.h"
#include "errno.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, CoAP Cloud!"
#define SLEEP_INTERVAL (1U * US_PER_SEC)

static ssize_t _coap_post_handler(const uint8_t *buf, size_t len, const sock_udp_ep_t *remote) {
    (void)buf;
    (void)len;
    (void)remote;
    printf("Response received\n");
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t res;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    int attempts = 0;

    while (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("IP address parsing failed\n");
        xtimer_sleep(1);
    }

    /** Initialize CoAP Request **/
    res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (res < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Add payload to the CoAP request
    if (coap_opt_add_payload_marker(&pdu) < 0) {
        printf("Failed to add payload marker\n");
        return 1;
    }

    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Continually try to send the request until successful
    while (1) {
        res = gcoap_req_send(buf, gcoap_get_request_length(&pdu), &remote, _coap_post_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (res >= 0) {
            printf("CoAP POST request sent successfully\n");
            break;
        } else {
            printf("Failed to send CoAP POST request. Retrying...\n");
            xtimer_sleep(SLEEP_INTERVAL);
        }
    }

    return 0;
}