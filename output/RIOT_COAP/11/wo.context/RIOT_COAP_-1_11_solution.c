#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>  // Ensure gcoap and CoAP related functions are included
#include <net/ipv6/addr.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // For sleep function (if needed)

#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // PDU buffer size
#define CONFIG_GCOAP_PORT 5683         // CoAP default port

#define MSG "Hello, COAP Cloud"
#define SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = CONFIG_GCOAP_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    
    // Convert server IP address
    if (inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse destination address");
        return -1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, MSG, strlen(MSG));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send the request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error sending message");
        return -1;
    } else {
        puts("Request sent successfully");
    }

    return 0;
}
