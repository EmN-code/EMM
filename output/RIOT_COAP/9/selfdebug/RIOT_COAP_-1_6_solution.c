#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <kernel_defines.h>
#include <stdio.h>
#include <string.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 0x02   // Define post method
#define CONFIG_GCOAP_PORT 5683

void _resp_handler(const void *memo, void *pdu, const void *remote) {
    (void)remote;  // Suppress unused variable warnings
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = CONFIG_GCOAP_PORT };

    // Convert IP address string to network format
    int result = inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    if (result != 1) {
        puts("Failed to parse IP address");
        return 1;
    }

    // Initialize CoAP request package
    coap_pkt_t pdu;
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    len += strlen("Hello, COAP Cloud");

    // Send the CoAP message
    ssize_t result_send = gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result_send <= 0) {
        puts("Failed to send CoAP request");
        return 1;
    }

    puts("CoAP request sent, waiting for response...");
    return 0;
}