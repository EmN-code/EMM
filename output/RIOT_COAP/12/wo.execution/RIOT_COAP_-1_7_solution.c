#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <string.h>

#define COAP_METHOD_POST 2 // Assuming POST has value 2 for this example
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define CONFIG_GCOAP_PDU_BUF_SIZE 256

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle responses here, for now we'll just print success
    printf("Response received\n");
}

int main() {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        fprintf(stderr, "inet_pton failed\n");
        return 1;
    }

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        fprintf(stderr, "Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}