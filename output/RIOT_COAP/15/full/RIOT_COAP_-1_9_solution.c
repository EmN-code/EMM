#include <mma8x5x.h>
#include <arpa/inet.h>
#include <ztimer.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <mutex.h>
#include <evtimer_mbox.h>
#include <stdatomic.h>
#include <stdio.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <net/emcute.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <dht.h>
#include <stddef.h>
#include <msg.h>
#include <unistd.h>
#include <esp_wifi.h>
#include <net/gcoap.h>
#include <timex.h>
#include <mma8x5x_params.h>
#include <architecture.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote){
    (void)remote;  // suppress unused variable warning
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout\n");
        return;
    }
    
    if (memo->state == GCOAP_MEMO_ERR) {
        printf("Error\n");
        return;
    }
    
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    int result;
    ssize_t len;

    while (1) {
        result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if(result <= 0) {
            printf("inet_pton failed\n");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH) == 0) {
        printf("gcoap_req_init failed\n");
        return 1;
    }

    coap_opt_add_format(&pdu, COAP_FORMAT_NONE);
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    if (coap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17) == 0) {
        printf("Error setting payload\n");
        return 1;
    }

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("gcoap_req_send error\n");
        return 1;
    }

    printf("Request sent\n");
    return 0;
}