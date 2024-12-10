#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <stdio.h>
#include <stdint.h>

#define SERVER_IP      "47.102.103.1"
#define SERVER_PORT    5683
#define URI_PATH       "/coap/test"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                             const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received\n");
    return 0;
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    uint8_t code;
    
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 16);

    ssize_t res = gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending request\n");
    }
    else {
        printf("Message sent\n");
    }

    return 0;
}