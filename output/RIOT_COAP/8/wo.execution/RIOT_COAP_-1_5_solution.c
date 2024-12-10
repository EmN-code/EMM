#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <dht.h>
#include <net/emcute.h>

#define GCOAP_REQ_BUF_SIZE 256
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(coap_pkt_t *pdu, const sock_udp_ep_t *remote, void *context) {
    printf("Received response\n");
}

int main(void) {
    uint8_t buf[GCOAP_REQ_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    // Convert the IP address
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: could not convert IP address\n");
        return -1;
    }

    // Initialize CoAP request
    int init_res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (init_res < 0) {
        printf("Error: failed to init CoAP request\n");
        return -1;
    }

    // Set payload
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Send request
    ssize_t send_res = gcoap_req_send(buf, pdu.used, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (send_res <= 0) {
        printf("Error: failed to send CoAP request\n");
        return -1;
    }

    printf("Sent CoAP request\n");

    return 0;
}