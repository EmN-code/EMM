#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) memo;
    (void) remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    memset(&remote, 0, sizeof(remote));
    remote.port = COAP_SERVER_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination address\n");
        return -1;
    }

    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH) == 0) {
        memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
        pdu.payload_len = strlen(PAYLOAD);

        if (gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
            printf("Error sending request\n");
            return -1;
        }
    } else {
        printf("Error initializing CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}