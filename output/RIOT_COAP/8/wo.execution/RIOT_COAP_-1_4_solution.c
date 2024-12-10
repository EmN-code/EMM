#include <unistd.h>
#include <time_units.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "sys/socket.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "stdio.h"

#define GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 0x02

int main(void) {
    const char *EMQX_SERVER_IP = "47.102.103.1";
    const char *EMQX_URI_PATH = "/coap/test";
    const char *PAYLOAD = "Hello, COAP Cloud";
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t result;

    // Convert IP address from string
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("inet_pton failed\n");
        return 1;
    }

    // Initialize the CoAP request - using POST
    result = gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (result == -1) {
        printf("gcoap_req_init failed\n");
        return 1;
    }

    // Copy payload into the PDU
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Set the endpoint port
    remote.port = 5683;

    // Send the request
    int len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    if (len == -1) {
        printf("coap_opt_finish failed\n");
        return 1;
    }
    result = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("gcoap_req_send failed\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}