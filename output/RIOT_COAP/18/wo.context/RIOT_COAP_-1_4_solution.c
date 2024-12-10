#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>

#define COAP_PORT 5683
#define SERVER_IP "47.102.103.1"
#define URI_PATH "/coap/test"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("timeout for msg id\n");
        return;
    }
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_PORT;

    // Using inet_pton to convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: Unable to parse destination address");
        return -1;
    }

    ssize_t result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        puts("Error: gcoap_req_init failed");
        return -1;
    }

    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    pdu.payload_len = strlen("Hello, COAP Cloud");

    result = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        puts("Error: gcoap_req_send failed");
        return -1;
    }

    puts("Request sent
");

    return 0;
}