#include <string.h>  // For memcpy
#include <stdio.h>   // For printf, sprintf
#include <net/sock/udp.h>  // For UDP related operations
#include <net/ipv4/addr.h>  // For IPv4 address handling
#include <net/gcoap.h>  // For gcoap API

#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define PAYLOAD "Hello, COAP Cloud!"

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize UDP endpoint
    memset(&remote, 0, sizeof(remote));
    remote.port = 5683;
    remote.family = AF_INET;

    // Convert IP address and validate
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return -1;
    }

    // Initialize CoAP request
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len <= 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    // Set CoAP request payload
    if (!memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD))) {
        printf("Payload set error\n");
        return -1;
    }

    // Finish adding options to the request
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }
    printf("CoAP request sent successfully\n");

    return 0;
}