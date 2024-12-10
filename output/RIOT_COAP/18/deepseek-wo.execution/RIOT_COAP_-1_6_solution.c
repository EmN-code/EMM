
#include <string.h>
#include <unistd.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <dht.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mbox.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(void *arg, int resp_code, void *resp_buf, size_t resp_len) {
    (void)arg;
    (void)resp_code;
    (void)resp_buf;
    (void)resp_len;
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL);

    return 0;
}
