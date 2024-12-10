#include <net/nanocoap/cache.h>
#include <gcoap.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* Initialize CoAP request */
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH) < 0) {
        puts("Error: unable to initialize CoAP request.");
        return -1;
    }

    const char *msg = "Hello, COAP Cloud";
    memcpy(pdu.payload, msg, strlen(msg));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    /* Define remote UDP endpoint */
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = SERVER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    /* Send CoAP request */
    if (gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error: unable to send CoAP request.");
        return -1;
    }

    puts("CoAP request sent successfully.");
    return 0;
}