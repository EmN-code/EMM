#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv4/addr.h>
#include <stdio.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
gcoap_request_memo_t memo;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = CONFIG_GCOAP_PORT };

    // Convert the server IP address
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    // Initialize the CoAP packet
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH) == -1) {
        printf("Error: unable to initialize packet\n");
        return -1;
    }

    // Set payload
    pdu.payload = (uint8_t *)PAYLOAD;
    pdu.payload_len = strlen(PAYLOAD);

    // Send the request
    ssize_t result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error: unable to send request\n");
        return -1;
    }

    printf("Request sent\n");
    return 0;
}