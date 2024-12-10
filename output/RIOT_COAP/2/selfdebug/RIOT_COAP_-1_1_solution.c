#include "thread_config.h"
#include "net/nanocoap/cache.h"
#include "gcoap.h"
#include <stdio.h>

static int send_coap_request(void) {
    /* Buffer to build the request */
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    
    /* Initialize CoAP packet structure */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    
    /* Set message payload */
    const char *msg = "Hello, COAP Cloud";
    size_t len = gcoap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, msg, strlen(msg));
    len += strlen(msg);

    /* Define the remote CoAP endpoint */
    sock_udp_ep_t remote = { .family = AF_INET6, .port = CONFIG_GCOAP_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, "47.102.103.1");
    
    /* Send the CoAP request */
    int res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Failed to send the CoAP request\n");
        return 1;
    }
    printf("CoAP request sent\n");
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Response timed out\n");
        return;
    }
    printf("Response received\n");
}

int main(void) {
    /* Initialize gcoap application */
    gcoap_register_listener(&_listener);
    
    /* Send CoAP request */
    return send_coap_request();
}