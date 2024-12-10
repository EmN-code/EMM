#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "mutex.h"
#include "msg.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "unistd.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    while (1) {
        gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        coap_hdr_set_type(&pdu, COAP_TYPE_NON);
        memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
        coap_hdr_set_payload_len(&pdu, strlen(COAP_MESSAGE));

        ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (res >= 0) {
            printf("CoAP request sent successfully\n");
            break;
        } else {
            printf("Failed to send CoAP request, retrying...\n");
            xtimer_sleep(1);
        }
    }

    return 0;
}