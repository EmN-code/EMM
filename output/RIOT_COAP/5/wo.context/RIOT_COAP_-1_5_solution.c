#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/sock.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handler implementation
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int result;

    // Initialize remote endpoint
    memset(&remote, 0, sizeof(remote));
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.port = CONFIG_GCOAP_PORT;

    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH) < 0) {
        puts("Error initializing request");
        return -1;
    }

    // Add payload
    ssize_t len = gcoap_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));
    if (len < 0) {
        puts("Error setting payload");
        return -1;
    }

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending request");
        return -1;
    }

    puts("Request sent successfully");
    return 0;
}