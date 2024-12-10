#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>  // Required for memcpy used in the example.
#include <stdatomic.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <timex.h>
#include <ztimer.h>
#include <time_units.h>
#include <dht.h>
#include <dht_params.h>

#define GCOAP_PDU_BUF_SIZE 256  // Properly define the buffer size macro.

static const char *EMQX_SERVER_IP = "47.102.103.1";
static const char *EMQX_URI_PATH = "/coap/test";
static const char *PAYLOAD = "Hello, COAP Cloud";

static sock_udp_ep_t remote;

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Handle response here.
}

int main(void)
{
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize request
    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Set payload;
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Convert IP address from string to binary form
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: Invalid IP address");
        return -1;
    }
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;

    // Send the request
    ssize_t res = gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error: Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP POST request sent\n");
    return 0;
}