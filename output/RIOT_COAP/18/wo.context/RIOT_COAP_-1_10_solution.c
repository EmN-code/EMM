#include <string.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define GCOAP_SOCKET_TYPE_UDP 0

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    /* Handle the response here */
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    const char *data = "Hello, COAP Cloud";
    size_t data_len = strlen(data);

    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    remote.port = 5683;

    coap_pkt_t pdu; // Declare a coap_pkt_t instance
    pdu.payload = (uint8_t *)data; // Assign payload data
    pdu.hdr = (pdu_t *)buf; // Initialize CoAP header buffer
    ssize_t res = gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res < 0) {
        // Handle error
    }

    return 0;
}