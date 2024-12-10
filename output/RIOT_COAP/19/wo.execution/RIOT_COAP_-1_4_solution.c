#include <time_units.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <dht_params.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <kernel_defines.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE (256U)
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT (5683)
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t* remote) {
    if (req_state == GCOAP_MEMO_COMPLETE) {
        printf("Response received\n");
    } else {
        printf("Request not completed: %d\n", req_state);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    // Set the remote address family
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = EMQX_PORT;
    
    // Convert IP address from string to byte form and store in remote addr
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    // Initialize CoAP packet
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    // Copy payload into PDU
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}