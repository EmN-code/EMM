#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define BUF_SIZE 128

int main(void) {
    /* Buffer to store CoAP request */
    uint8_t buf[BUF_SIZE];

    /* Initialize CoAP packet and request parameters */
    coap_pkt_t pdu;
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_IP, &server.addr.ipv6);

    /* Preparing the payload */
    const char* payload = "Hello, COAP Cloud";

    /* Initialize CoAP request */
    int len = gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }

    /* Set CoAP payload */
    if (!coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE)) {
        memcpy(pdu.payload, payload, strlen(payload));
        pdu.payload_len = strlen(payload);
    }

    /* Send CoAP request */
    if (gcoap_req_send(&buf[0], len, &server, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}