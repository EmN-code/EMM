#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <string.h>
#include <arpa/inet.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote;

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Set Payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Set Remote Address
    remote.family = AF_INET;
    remote.port = 5683;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("inet_pton() failed");
        return -1;
    }

    // Send CoAP request and print number of bytes sent
    ssize_t nbytes = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    printf("Sent %ld bytes\n", nbytes);

    return 0;
}