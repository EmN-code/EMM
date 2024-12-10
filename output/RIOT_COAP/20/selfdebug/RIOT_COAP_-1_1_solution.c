#include <net/nanocoap/cache.h>
#include <gcoap.h>
#include <net/gnrc.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST
#define MESSAGE_CONTENT "Hello, COAP Cloud"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP_TRUNC) {
        puts("GCOAP: response too large, truncated");
    } else if (memo->state != GCOAP_MEMO_COMPLETE) {
        printf("GCOAP: response error, state: %d\n", memo->state);
    } else {
        printf("GCOAP: request successful, response code: %u\n", coap_get_code(pdu));
    }

    return GCOAP_MEMO_DONE;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET6;
    remote.port = SERVER_PORT;
    if (gnrc_ipv6_addr_from_str(&remote.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return -1;
    }

    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD, URI_PATH) < 0) {
        puts("Error: unable to initialize CoAP request");
        return -1;
    }

    memcpy(pdu.payload, MESSAGE_CONTENT, strlen(MESSAGE_CONTENT));
    pdu.payload_len = strlen(MESSAGE_CONTENT);

    if (gcoap_req_send(buf, pdu.payload_len + coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error: unable to send CoAP request");
        return -1;
    }

    puts("CoAP request sent successfully");
    return 0;
}