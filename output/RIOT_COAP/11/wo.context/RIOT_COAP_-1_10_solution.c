#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include <stdio.h>
#include <string.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST           2
#define EMQX_URI_PATH              "/coap/test"
#define EMQX_SERVER_IP             "47.102.103.1"

static void _resp_handler(const void *pdu, unsigned pdu_len, void *arg){
    (void)pdu;
    (void)pdu_len;
    (void)arg;
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t result;

    memset(&remote, 0, sizeof(remote));
    remote.port = CONFIG_GCOAP_PORT;
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
