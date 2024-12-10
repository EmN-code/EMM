#include <stdio.h> 
#include <string.h>
#include <net/gcoap.h>
#include <unistd.h> 
#include <thread_config.h>

static size_t coap_build_pdu(unsigned char *buf, size_t len) {
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, len, COAP_METHOD_POST, "/coap/test");
    size_t pdu_len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    const char* payload = "Hello, COAP Cloud";
    pdu.payload = (uint8_t *)payload;
    pdu.payload_len = strlen(payload);
    return pdu_len + pdu.payload_len;
}

int main(void) {
    unsigned char buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET6, .port = CONFIG_GCOAP_PORT };
    inet_pton(AF_INET6, "47.102.103.1", &remote.addr);

    size_t len = coap_build_pdu(buf, sizeof(buf));
    int res = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    if (res == GCOAP_MEMO_RESP_TRUNC) {
        printf("Response was truncated\n");
    } else if (res < 0) {
        printf("Failed to send request\n");
    } else {
        printf("CoAP request sent\n");
    }
    
    return 0;
}