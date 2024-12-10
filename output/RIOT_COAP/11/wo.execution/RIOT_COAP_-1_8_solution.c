#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>

#define COAP_METHOD_POST 0x02
#define COAP_PORT "5683"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static ssize_t _resp_handler(coap_pkt_t* pdu, void* context) {
    puts("Response received");
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;
    sock_udp_ep_t remote;

    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.port = atoi(COAP_PORT);

    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        puts("Error in request initialization");
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending request");
        return 1;
    }

    puts("Request sent");

    return 0;
}