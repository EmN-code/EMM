#include "mma8x5x.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdlib.h"
#include "mutex.h"
#include "evtimer_mbox.h"
#include "stdatomic.h"
#include "stdio.h"
#include "mbox.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "stddef.h"
#include "dht.h"
#include "msg.h"
#include "unistd.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "timex.h"
#include "mma8x5x_params.h"
#include "ipv6/addr.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define BUF_SIZE    (128U)

static void response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;

    if (pdu == NULL) {
        printf("No response received\n");
    }
    else {
        printf("Response received\n");
    }
}

int main(void) {
    uint8_t buf[BUF_SIZE];
    ssize_t bytes;
    sock_udp_ep_t server_sock = {.family = AF_INET, .port = COAP_PORT};

    // Convert IP address string to byte representation
    int res = inet_pton(AF_INET, COAP_SERVER_IP, &server_sock.addr.ipv4);
    if (res <= 0) {
        printf("inet_pton() failed\n");
        return -1;
    }

    // Initiate CoAP message
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    // Add payload marker
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    size_t payload_len = coap_opt_add_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17);

    // Get total length of request
    size_t len = gcoap_get_request_length(&pdu, payload_len);

    while (1) {
        // Send CoAP request
        bytes = gcoap_req_send(buf, len, &server_sock, response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (bytes <= 0) {
            printf("Could not send CoAP request\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue; // Retry on failure
        }
        else {
            printf("CoAP request sent\n");
            break;
        }
    }

    return 0;
}