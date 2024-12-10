#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define COAP_METHOD_POST    (0x02)
#define CONFIG_GCOAP_PDU_BUF_SIZE (128)
#define EMQX_SERVER_IP      "47.102.103.1"
#define EMQX_URI_PATH       "/coap/test"
#define PAYLOAD  "Hello, COAP Cloud"

void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    (void)req_state;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);  // Error when inet_pton not declared
    remote.family = AF_INET;
    remote.port = 5683;

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH); // Error when gcoap_req_init not declared
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD) + 1); // Error if pdu payload not recognized

    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP); // Error when gcoap_req_send not declared

    return 0;
}