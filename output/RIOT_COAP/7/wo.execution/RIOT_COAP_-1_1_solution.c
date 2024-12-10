#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net/nanocoap.h"
#include "net/gnrc.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static const char *msg = "Hello, COAP Cloud";

int main(void) {
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);

    if (!netif) {
        printf("No network interface found\n");
        return 1;
    }

    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .netif = netif->pid
    };
    ipv6_addr_from_str((ipv6_addr_t *) &remote.addr, SERVER_IP);
    remote.port = SERVER_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    memcpy(coap_get_payload(&pdu), msg, strlen(msg));
    gcoap_req_send(&buf[0], pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("CoAP POST request sent to %s%s\n", SERVER_IP, URI_PATH);

    return 0;
}