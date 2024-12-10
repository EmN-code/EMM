#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <string.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH    "/coap/test"
#define PAYLOAD     "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    ssize_t res;

    /* Configure remote endpoint */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse destination address");
        return -1;
    }

    /* Initialize CoAP request */
    res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        puts("Error: unable to initialize request");
        return res;
    }

    /* Set payload */
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    /* Send the CoAP request */
    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, 
                         GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error: unable to send request");
        return res;
    }

    puts("CoAP request sent successfully");
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    puts("Response received");
}