#include <ztimer.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/ipv4/addr.h>
#include <kernel_defines.h>
#include <msg.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <stddef.h>
#include <stdio.h>
#include <mbox.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <time_units.h>
#include <unistd.h>
#include <dht.h>
#include <esp_wifi.h>
#include <dht_params.h>
#include <timex.h>

#define COAP_METHOD_POST 0x02
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683

void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_COMPLETE) {
        puts("Response received");
    }
    else {
        puts("Failed to receive response");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    int result;

    /* Convert IP address */
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Failed to convert IP address");
        return -1;
    }

    while (1) {
        int res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
        if (res < 0) {
            printf("Failed to initialize request: %d\n", res);
            xtimer_sleep(1);
            continue;
        }

        const char *payload = "Hello, COAP Cloud";
        size_t len = gcoap_pdu_fill_from_str(&pdu, (const uint8_t *)payload);

        res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Failed to send request: %d\n", res);
            xtimer_sleep(1);
            continue;
        }
        else {
            puts("Request sent");
        }

        break;  // Exit the loop after a successful send
    }

    return 0;
}

size_t gcoap_pdu_fill_from_str(coap_pkt_t *pdu, const uint8_t *payload) {
    size_t len = strlen((const char *)payload);
    memcpy(pdu->payload, payload, len);
    pdu->payload_len = len;
    return len;
}