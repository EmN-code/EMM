#include <string.h>  // Include necessary headers in descending order by first letter
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

int main(void) {
    // Define buffer and remote endpoint
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .port = 5683 };

    // Initialize CoAP request
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Correctly use inet_pton for setting the IP address
    remote.family = AF_INET;
    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        // Handle error
        return -1;
    }

    // Send CoAP request
    ssize_t res = gcoap_req_send(&buf[0], pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        // Handle send error
        return -1;
    }

    return 0;
}