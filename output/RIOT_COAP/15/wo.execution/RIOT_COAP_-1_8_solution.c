#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <arpa/inet.h>

#define COAP_METHOD_POST       0x02
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

#define EMQX_SERVER_IP         "47.102.103.1"
#define EMQX_SERVER_PORT       5683
#define EMQX_URI_PATH          "/coap/test"
#define PAYLOAD                "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request: %d\n", (int)res);
    }

    return 0;
}