#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "msg.h"
#include "mutex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "kernel_defines.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define RETRY_INTERVAL 1000000 // in microseconds

// Function to initialize and send a CoAP POST request
static int send_coap_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;
    int retries = 0;

    // Setting up the remote server endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to encode IP address\n");
        return -1;
    }

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_pkt_set_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Retry loop for sending the request
    while (retries < 3) {
        res = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len,
                             &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res <= 0) {
            printf("Error: failed to send CoAP request\n");
            retries++;
            xtimer_usleep(RETRY_INTERVAL);
        } else {
            printf("CoAP request sent successfully\n");
            return 0;
        }
    }
    return -1;
}

int main(void) {
    // Initialize networking stack here (and potential device-specific setup)
    // For demo purposes, we only focus on sending the CoAP request
    int status = send_coap_request();
    if (status != 0) {
        printf("CoAP request failed after retries\n");
    }
    return 0;
}