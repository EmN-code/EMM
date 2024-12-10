#include <net/ipv4/addr.h>
#include <net/nanocoap.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response received, code: %d\n", coap_get_code(pdu));
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = CONFIG_GCOAP_PORT;
    
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    size_t len = gcoap_payload(pdu, &buf[0], strlen(payload));
    strncpy((char *)buf, payload, len);
    
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    return 0;
}