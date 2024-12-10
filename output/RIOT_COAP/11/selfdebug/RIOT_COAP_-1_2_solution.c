#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

#define COAP_PDU_BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

static ssize_t _send_coap_msg(void) {
    uint8_t buf[COAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

    if (inet_pton(AF_INET6, SERVER_IP, &remote.addr) == 0) {
        printf("Error: unable to parse destination address\n");
        return -1;
    }

    gcoap_req_init(&pdu, buf, COAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(&pdu, COAP_TYPE_NON);
    coap_body(&pdu, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    if (gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error: unable to send\n");
        return -1;
    }

    printf("CoAP msg sent: Hello, COAP Cloud\n");
    return 0;
}

int main(void) {
    msg_init_queue(msg_q, ARRAY_SIZE(msg_q));
    gcoap_init();

    _send_coap_msg();

    return 0;
}
