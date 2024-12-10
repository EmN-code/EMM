#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    size_t payload_len;
    const uint8_t *payload = coap_get_payload(pdu, &payload_len);
    if (payload) {
        printf("Got response: %.*s\n", (int)payload_len, (char *)payload);
    } else {
        printf("Got response with no payload\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sockaddr_t remote;
    
    // CoAP server settings
    char *server_ip = "47.102.103.1";
    uint16_t port = 5683;

    // Initialize remote endpoint
    remote.family = AF_INET;
    remote.port = port;
    inet_pton(AF_INET, server_ip, &remote.addr.ipv4);

    // Initialize CoAP PDU
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    // Set payload
    size_t len = strlen("Hello, COAP Cloud");
    memcpy(coap_payload(&pdu), "Hello, COAP Cloud", len);
    coap_hdr_set_type(&pdu, COAP_TYPE_CON); // Optional, type confirmation
 
    printf("Sending CoAP POST request to %s:%d with payload: 'Hello, COAP Cloud'\n", server_ip, port);
  
    // Send the request
    ssize_t res = gcoap_req_send(buf, pdu.used, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    return 0;
}