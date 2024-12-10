#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <dht.h>
#include <dht_params.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <net/gcoap.h>

#define COAP_METHOD_POST 0x02
#define EMQX_URI_PATH "/coap/test"
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define RETRY_DELAY (1U * US_PER_SEC)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) memo;
    (void) remote;
    if (pdu->payload_len) {
        char content[64];
        memcpy(content, pdu->payload, pdu->payload_len);
        content[pdu->payload_len] = '\0';
        printf("Response: %s\n", content);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int res;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    while (1) {
        res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
        if (res != 1) {
            printf("Failed to convert IP. Retrying in %u seconds...\n", RETRY_DELAY / US_PER_SEC);
            xtimer_usleep(RETRY_DELAY);
            continue;
        }
        
        gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
        coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
        const char* msg = "Hello, COAP Cloud";
        memcpy(pdu.payload, msg, strlen(msg));
        pdu.payload_len = strlen(msg);
        coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

        if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
            printf("Failed to send request. Retrying...");
            xtimer_usleep(RETRY_DELAY);
            continue;
        }
        printf("Request sent
");
        break;
    }

    return 0;
}