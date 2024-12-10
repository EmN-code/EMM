#include "netinet/in.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "dht_params.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "sys/socket.h"
#include "arpa/inet.h"

#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

void _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)req_state;
    (void)remote;
    if (!pdu) {
        printf("gcoap: timeout
");
        return;
    }
    printf("CoAP Response Received!");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    const char *EMQX_SERVER_IP = "47.102.103.1";
    const char *EMQX_URI_PATH = "/coap/test";
    uint8_t payload[] = "Hello, COAP Cloud";

    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;
    
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to encode destination address
");
        return -1;
    }

    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len <= 0) {
        printf("Error: unable to initialize request
");
        return -1;
    }

    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) != 1) {
        printf("Error: unable to finish opt
");
        return -1;
    }

    len += coap_payload(&pdu, payload, sizeof(payload));

    ssize_t result = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error: failed to send CoAP request
");
        return -1;
    }
    return 0;
}