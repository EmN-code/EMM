#include <unistd.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;  // Suppress unused variable warning when not needed
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("GCOAP_MEMO_TIMEOUT\n");
        return;
    }

    unsigned code = coap_get_code(pdu);
    printf("Response code: %u\n", code);
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    res = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Error: could not convert IP address\n");
        return 1;
    }

    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    gcoap_opt_finish(&pdu, len, COAP_OPT_FINISH_NONE);

    len += strlen(PAYLOAD);

    if ((res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP)) <= 0) {
        printf("Error sending CoAP request\n");
    } else {
        printf("CoAP request sent\n");
    }
    
    return 0;
}