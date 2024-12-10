#include <ztimer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <stdio.h>
#include <dht_params.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <stdatomic.h>
#include <stddef.h>
#include <msg.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>
#include <math.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <netinet/in.h>

#define EMQX_SERVER_IP     "47.102.103.1"
#define EMQX_PORT          5683
#define EMQX_URI_PATH      "/coap/test"
#define COAP_METHOD_POST   0x02
#define COAP_BUF_SIZE      256

// Handler to process CoAP response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote; // suppress unused variable warning
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Response timeout");
        return;
    }
    switch (coap_get_code(pdu)) {
        case COAP_CODE_204:
            puts("Success");
            break;
        default:
            puts("Failure");
            break;
    }
}

int main(void) {
    puts("RIOT CoAP Client");

    uint8_t buf[COAP_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize UDP socket for CoAP
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_PORT };
    int res = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error in IP address formatting");
        return 1;
    }

    // Prepare CoAP PDU
    size_t len = gcoap_req_init(&pdu, buf, COAP_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Set CoAP payload
    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send CoAP request
    ssize_t bytes_sent;
    while (1) {
        bytes_sent = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (bytes_sent <= 0) {
            puts("Failed to send CoAP request, retrying...");
            ztimer_sleep(ZTIMER_USEC, 1000000);
            continue;
        }
        puts("CoAP request sent successfully");
        break;
    }

    return 0;
}