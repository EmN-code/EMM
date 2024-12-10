#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo; // Suppress unused variable warning
    (void)pdu;
    (void)remote;
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Invalid server IP");
        return 1;
    }
    
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH) == -1) {
        puts("Failed to initialize request");
        return 1;
    }
    size_t len = coap_put_option(&pdu, COAP_OPT_CONTENT_FORMAT, 0, NULL);
    len += coap_put_block(&pdu, 0, (const uint8_t *)MESSAGE, strlen(MESSAGE));
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request: %d\n", res);
        return 1;
    }

    puts("Request sent");
    return 0;
}
