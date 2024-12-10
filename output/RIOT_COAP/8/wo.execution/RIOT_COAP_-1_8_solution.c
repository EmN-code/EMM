#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht.h>
#include <dht_params.h>
#include <kernel_defines.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_METHOD_POST 2
#define COAP_PAYLOAD "Hello, COAP Cloud"
#define GCOAP_MEMORY_COUNT 5

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("Request timeout\n");
    }
    else if (req_state == GCOAP_MEMO_BAD_RESPONSE) {
        printf("Bad response\n");
    }
    else {
        printf("Response received: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
}

int main(void) {
    static const uint8_t buffer[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    size_t len;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    len = gcoap_req_init(&pdu, (uint8_t *)buffer, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    if (gcoap_req_send(buffer, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");

    return 0;
}
