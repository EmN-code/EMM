#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response here (e.g., log it or verify it)
}

int main(void) {
    // Define variables for CoAP request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Allocate memory for the request and initialize the remote endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;
    if(inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Error: Invalid IP address format\n");
        return -1;
    }

    // Initialize CoAP message
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    // Copy the payload to the PDU
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    // Send CoAP request
    gcoap_req_send(&buf[0], pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}