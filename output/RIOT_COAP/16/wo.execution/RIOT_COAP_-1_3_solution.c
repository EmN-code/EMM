#include "unistd.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "sys/socket.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "dht_params.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _response_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    printf("Response received\n");
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    size_t buf_len = CONFIG_GCOAP_PDU_BUF_SIZE;
    uint8_t buf[buf_len];

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, buf_len, COAP_METHOD_POST, COAP_URI_PATH);

    size_t payload_len = strlen(PAYLOAD);
    memcpy(coap_payload(&pdu), PAYLOAD, payload_len);

    ssize_t bytes_sent = gcoap_req_send(buf, pdu.hdr->len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error sending packet.\n");
        return 1;
    }

    printf("Request sent to %s\n", COAP_SERVER_IP);
    return 0;
}