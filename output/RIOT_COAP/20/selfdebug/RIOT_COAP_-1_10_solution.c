#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define COAP_METHOD_POST 0x02
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define COAP_PORT 5683
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    // Buffer for CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // SOCK address setup
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };

    // Set server IP address
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error in IP address conversion\n");
        return -1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Set payload of the CoAP packet
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    ssize_t bytes_sent = gcoap_req_send(buf, sizeof(buf), &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Failed to send COAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    return 0;
}