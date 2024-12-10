#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>
#include "ztimer.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht.h"
#include "dht_params.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "timex.h"
#include "time_units.h"
#include <mutex.h>
#include <stdatomic.h>
#include <unistd.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define CONFIG_MY_GCOAP_PDU_BUF_SIZE 128

static void response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // Prevent unused variable warning

    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Request timed out\n");
        return;
    }

    printf("Response received:\n");
}

int main(void) {
    uint8_t buf[CONFIG_MY_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    // Initialize the GCoAP PDU
    gcoap_req_init(&pdu, buf, CONFIG_MY_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    // Fill PDU with payload
    size_t len = snprintf((char *)pdu.payload, pdu.payload_len, "Hello, COAP Cloud");

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    while (1) {
        res = gcoap_req_send(buf, len, &remote, response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        
        if (res > 0) {
            printf("Request sent\n");
            break;
        } else {
            printf("Failed to send request, retrying...\n");
            xtimer_sleep(1);  // wait a second before retry
        }
    }

    return 0;
}