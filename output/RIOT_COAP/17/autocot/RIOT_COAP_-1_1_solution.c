#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "kernel.h"
#include "msg.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP) == NULL) {
        puts("Error: unable to parse server address");
        return -1;
    }
    
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    gcoap_req_send(buf, pdu.payload_len + gcoap_opt_finish(&pdu, COAP_OPT_FINISH_NONE), &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    puts("CoAP POST request sent to CoAP server");

    return 0;
}