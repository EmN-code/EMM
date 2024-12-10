#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response received with code %u\n", coap_get_code_class(pdu) * 100 + coap_get_code_detail(pdu));
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6 };

    // Configure the remote server's endpoint
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    // Initialize a CoAP POST request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH) == -1) {
        printf("Error initializing request\n");
        return -1;
    }

    // Add payload to the CoAP request
    if (!coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE)) {
        printf("Error finishing CoAP options\n");
        return -1;
    }
    if (coap_add_block(&pdu, COAP_MESSAGE, strlen(COAP_MESSAGE), 0) == -1) {
        printf("Error adding block to CoAP message\n");
        return -1;
    }

    // Send the CoAP request to the server
    if (gcoap_req_send(buf, pdu.payload - buf, &remote, response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) == -1) {
        printf("Error sending request\n");
        return -1;
    }

    printf("CoAP POST request sent to %s\n", COAP_SERVER_IP);

    return 0;
}