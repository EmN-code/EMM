// Import necessary headers in descending order
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <stdio.h>

// Define configuration constants
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT (5683)
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"
#define BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

uint8_t buf[BUF_SIZE];

// Method declarations
void send_coap_request(void);
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    send_coap_request();
    return 0;
}

void send_coap_request(void) {
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error converting IP address\n");
        return;
    }
    
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    len += coap_payload_put(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE));
    
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        unsigned code = gcoap_get_code(pdu);
        printf("Response received with code %u\n", code);
    } else {
        printf("No response received\n");
    }
}