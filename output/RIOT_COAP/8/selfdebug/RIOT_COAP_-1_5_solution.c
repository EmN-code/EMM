#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

// Define configuration settings only once
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

int main(void) {
    // Configuration for the CoAP message and network setup
    const char *coap_server_ip = "47.102.103.1";
    const char *uri_path = "/coap/test";
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    ssize_t res;

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, uri_path);

    // Set target CoAP server address
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = CONFIG_GCOAP_PORT;

    // Use inet_pton to convert dotted-quad string to network byte order
    if (inet_pton(AF_INET, coap_server_ip, &remote.addr.ipv4) != 1) {
        puts("inet_pton error");
        return 1;
    }

    // Set payload to "Hello, COAP Cloud"
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Send the CoAP POST request
    res = gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res <= 0) {
        puts("CoAP request send error");
        return 1;
    }

    puts("CoAP request sent successfully");
    return 0;
}