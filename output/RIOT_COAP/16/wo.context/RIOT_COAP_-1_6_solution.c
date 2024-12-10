#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>   // Including for inet_pton function
#include <net/gcoap.h>   // Including for gcoap related functions and definitions
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu) {
        printf("Received response: %u\n", coap_get_code(pdu));
    }
    else {
        puts("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    ssize_t result;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, sizeof(COAP_MESSAGE));
    coap_hdr_set_type(&pdu, COAP_TYPE_CON);
    coap_hdr_set_id(&pdu, coap_get_id());

    result = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (result <= 0) {
        printf("Error sending request or no response (Error: %d)\n", (int)result);
    } else {
        printf("Request sent successfully\n");
    }

    return 0;
}
