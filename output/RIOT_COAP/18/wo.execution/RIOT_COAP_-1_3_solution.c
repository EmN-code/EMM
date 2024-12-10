#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "kernel_defines.h"
#include "mma8x5x_regs.h"
#include "arpa/inet.h"

#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define SERVER_IP "47.102.103.1"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;  // suppress unused variable warning
    (void)remote;
    if (pdu != NULL) {
        printf("Received response: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse server IP\n");
        return -1;
    }

    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (len <= 0) {
        printf("Error: unable to initialize request\n");
        return -1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");

    while (true) {
        // This loop can be expanded to handle other events in the application
    }

    return 0;
}