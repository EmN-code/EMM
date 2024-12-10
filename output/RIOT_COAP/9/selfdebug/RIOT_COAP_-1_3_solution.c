#include <net/sock/udp.h>
#include <net/sock.h>
#include <arpa/inet.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

gcoap_request_memo_t req_memo;
sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received \n");
    } else {
        printf("Error receiving response \n");
    }
}

static void send_coap_request(void) {
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IPV4 address");
        return;
    }
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    size_t payload_len = strlen(COAP_PAYLOAD);
    memcpy(coap_payload(&pdu), COAP_PAYLOAD, payload_len);
    coap_hdr_set_type(&pdu, COAP_TYPE_CON);

    ssize_t len = gcoap_req_send(buf, pdu.hdr_len + payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        printf("Failed to send request\n");
    }
}

int main(void) {
    send_coap_request();
    return 0;
}