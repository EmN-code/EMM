#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Handle the response from the server */
    if (pdu) {
        printf("Response received: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t len;

    /* Parse server IP address */
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to parse IP address\n");
        return -1;
    }

    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    /* Initialize CoAP packet */
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    /* Add payload to CoAP packet */
    const char payload[] = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, sizeof(payload) - 1);
    pdu.payload_len = sizeof(payload) - 1;

    /* Send CoAP request */
    gcoap_req_send(buf, len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    printf("CoAP request sent\n");

    return 0;
}