#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/nanocoap.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void my_inet_pton(const char *src, uint8_t *dst) {
    sscanf(src, "%hhu.%hhu.%hhu.%hhu", &dst[0], &dst[1], &dst[2], &dst[3]);
}

int main(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;
    size_t len;

    sock_udp_ep_t remote = {0};
    remote.port = SERVER_PORT;
    my_inet_pton(SERVER_IP, remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    strncpy((char *)coap_get_payload(&pdu), MESSAGE, sizeof(buf) - (pdu.payload - buf));
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    ssize_t res = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP POST request sent successfully");
    return 0;
}
