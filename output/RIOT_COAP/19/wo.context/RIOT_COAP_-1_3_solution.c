#include <net/ipv4/addr.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP           "47.102.103.1"
#define SERVER_PORT         5683
#define URI_PATH            "/coap/test"
#define PAYLOAD             "Hello, COAP Cloud"

gcoap_request_memo_t memo;
uint8_t buffer[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t server_endpoint;
char addr_str[IPV6_ADDR_MAX_STR_LEN];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received from server\n");
}

int main(void) {
    coap_pkt_t pdu;
    int len;

    if (inet_pton(AF_INET, SERVER_IP, &server_endpoint.addr.ipv4) != 1) {
        printf("Error converting server address\n");
        return 1;
    }
    server_endpoint.port = SERVER_PORT;

    gcoap_req_init(&pdu, buffer, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    if(gcoap_req_send(buffer, len, &server_endpoint, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Failed to send request\n");
        return 1;
    }

    printf("Request sent successfully\n");
    return 0;
}