#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "net/nanocoap.h"
#include "net/gnrc/netif.h"
#include "net/gnrc.h"
#include "msg.h"

#define COAP_INBUF_SIZE 128
#define COAP_OUTBUF_SIZE 128
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"
#define URI_PATH "/coap/test"

static ssize_t send_coap_request(char *buf, size_t len) {
    coap_pkt_t pktr;
    coap_pkt_init(&pktr, (uint8_t *)buf, len, COAP_METHOD_POST, COAP_TYPE_CON);
    coap_hdr_set_id(&pktr, coap_get_id());
    coap_opt_add_uri_path(&pktr, URI_PATH);
    const uint8_t payload[] = "Hello, COAP Cloud";
    coap_payload(&pktr, (uint8_t *)payload, sizeof(payload) - 1);

    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    remote.port = atoi(SERVER_PORT);
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);

    ssize_t res = sendto(sockfd, buf, pktr.payload_len, 0, (sock_addr_t *)&remote, sizeof(remote));
    return res;
}

int main(void) {
    static char inbuf[COAP_INBUF_SIZE];
    static char outbuf[COAP_OUTBUF_SIZE];
    msg_init_queue(inbuf, COAP_INBUF_SIZE);

    printf("Sending CoAP request\n");
    ssize_t res = send_coap_request(outbuf, sizeof(outbuf));

    if (res < 0) {
        puts("Failed to send request");
    } else {
        puts("Request sent successfully");
    }

    return 0;
}