#include "netinet/in.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "mma8x5x_regs.h"
#include "mutex.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    printf("Response from server: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    ssize_t bytes_sent;
    int len;

    printf("Starting CoAP client\n");

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    pdu.payload_len = strlen(MESSAGE);

    bytes_sent = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes_sent < 0) {
        printf("Failed to send request\n");
    }

    return 0;
}
