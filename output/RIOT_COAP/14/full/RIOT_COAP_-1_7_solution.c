#include "ztimer.h"
#include "xtimer.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "stdint.h"
#include "stdlib.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "arpa/inet.h"

#define COAP_PORT      5683
#define SERVER_ADDR    "47.102.103.1"
#define URI_PATH      "/coap/test"
#define PAYLOAD       "Hello, COAP Cloud"

gcoap_req_t req;
sock_udp_ep_t remote;

static int send_coap_post(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t len;

    /* Initialize request PDU */
    coap_pkt_t pdu;
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Error initializing CoAP request\n");
        return -1;
    }

    /* Set the request payload */
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));
    coap_hdr_set_payload_len(&pdu, sizeof(PAYLOAD));

    /* Set up the remote endpoint */
    remote.family = AF_INET;
    remote.port = COAP_PORT;
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error parsing server address\n");
        return -1;
    }

    ssize_t res;
    while (1) {
        res = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Sending CoAP request failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        } else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }

    return 0;
}

int main(void) {
    /* Send the CoAP POST request */
    if (send_coap_post() < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("Finished sending CoAP request\n");
    return 0;
}