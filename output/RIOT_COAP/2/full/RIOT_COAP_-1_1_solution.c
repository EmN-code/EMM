#include <stdio.h>
#include <stdint.h>
#include "net/socket.h"
#include "net/gcoap.h"

// CoAP server details
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

#define BUF_SIZE 128

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout from server \n");
        return;
    }
    printf("Response received: %u\n", coap_get_code(pdu));
}

int send_coap_request(void) {
    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t server = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    const char* msg = "Hello, COAP Cloud";
    size_t len = strlen(msg);
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) < 0) {
        printf("Error finishing options\n");
        return -1;
    }
    memcpy(pdu.payload, msg, len);
    pdu.payload_len = len;
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR);
    server.port = SERVER_PORT;
    return gcoap_req_send(buf, len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void) {
    printf("Starting CoAP client\n");
    while (1) {
        send_coap_request();
        xtimer_sleep(10); // wait 10 seconds before sending the next request
    }
    return 0;
}