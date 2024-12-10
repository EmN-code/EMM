#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <arpa/inet.h>
#include <dht.h>
#include <dht_params.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <msg.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

void _resp_handler(unsigned, coap_pkt_t*, sock_udp_ep_t*);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Set remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    while (1) {
        // Initialize CoAP request
        int err = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
        if (err < 0) {
            printf("gcoap_req_init() failed\n");
            xtimer_sleep(1);
            continue;
        }

        // Set payload
        memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
        coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

        // Send CoAP request
        err = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (err < 0) {
            printf("gcoap_req_send() failed\n");
            xtimer_sleep(1);
            continue;
        }

        printf("CoAP request sent successfully\n");
        break;
    }
    return 0;
}

void _resp_handler(unsigned req_state, coap_pkt_t *pdu, sock_udp_ep_t *remote) {
    (void)remote;  // Unused parameter
    printf("Response received\n");
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("CoAP request timed out\n");
        return;
    }
    else if (req_state == GCOAP_MEMO_ERR) {
        printf("CoAP request error\n");
        return;
    }
    // Process valid response...
}