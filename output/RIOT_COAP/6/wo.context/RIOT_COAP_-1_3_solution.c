#include net/sock/udp.h
#include net/gcoap.h
#include net/af.h

#define SERVER_PORT 5683
#define SERVER_IP "47.102.103.1"
#define COAP_METHOD_POST 2
#define EMQX_URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void) {
    // Allocate buffer for PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Adding payload to CoAP packet
    coap_pkt_t payload_pdu;
    payload_pdu.payload = (uint8_t*)MESSAGE;
    payload_pdu.payload_len = sizeof(MESSAGE);
    memcpy(pdu.payload, payload_pdu.payload, payload_pdu.payload_len);

    sock_udp_ep_t remote;
    remote.port = SERVER_PORT;

    // Resolve IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination IP address\n");
        return 1;
    }

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, payload_pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error: failed to send CoAP request\n");
    } else {
        printf("CoAP POST request sent successfully\n");
    }

    return 0;
}