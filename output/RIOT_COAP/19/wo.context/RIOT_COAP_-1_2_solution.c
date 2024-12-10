#include <net/gcoap.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE (4U)
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT (5683)
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static msg_t _msg_queue[MSG_QUEUE_SIZE];

int main(void) {
    printf("CoAP example requesting POST to server [%s]%s\n", COAP_SERVER_IP, EMQX_URI_PATH);

    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;

    // Convert the server IP string to a usable format
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Set the payload of the CoAP packet
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent\n");
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response handler called\n");
}