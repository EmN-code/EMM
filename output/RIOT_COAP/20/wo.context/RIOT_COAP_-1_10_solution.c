#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/coap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse the server IP address");
        return -1;
    }

    printf("Sending CoAP request to %s:%u\n", COAP_SERVER_IP, COAP_SERVER_PORT);

    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH) == 0) {
        puts("Error: unable to initialize CoAP request");
        return -1;
    }

    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    if (gcoap_req_send(buf, pdu.hdr.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) == -1) {
        puts("Error: unable to send CoAP request");
        return -1;
    }

    puts("CoAP POST request sent successfully");
    return 0;
}