#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE      (128)

// Function to initialize and send a CoAP request
void send_coap_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];  // Buffer for the PDU
    coap_pkt_t pdu;
    const char *EMQX_SERVER_IP = "47.102.103.1";
    const char *EMQX_URI_PATH = "/coap/test";
    const char *PAYLOAD = "Hello, COAP Cloud";
    int len;

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len = strlen(PAYLOAD);

    // Set up the remote endpoint
    sock_udp_ep_t remote;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.port = 5683;

    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void) {
    send_coap_request();
    return 0;
}