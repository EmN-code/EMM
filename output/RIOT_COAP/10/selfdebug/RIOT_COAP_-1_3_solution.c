#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    int result;

    /* Convert IP address from text to binary form */
    result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: inet_pton failed\n");
        return -1;
    }

    /* Initialize CoAP packet */
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH) == 0) {
        printf("Error: gcoap_req_init failed\n");
        return -1;
    }

    /* Set payload */
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    /* Send CoAP request */
    ssize_t bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error: gcoap_req_send failed\n");
        return -1;
    }

    printf("CoAP POST request sent successfully\n");

    return 0;
}