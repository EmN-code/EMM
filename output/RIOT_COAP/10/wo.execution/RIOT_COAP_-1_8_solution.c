#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <sched.h>
#include <netinet/in.h>
#include <string.h>  // This is required for memcpy and strlen functions
#include <arpa/inet.h>
#include <msg.h>
#include <net/nanocoap/cache.h>
#include <mbox.h>
#include <mutex.h>
#include <dht.h>
#include <dht_params.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <ztimer.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define PAYLOAD "Hello, COAP Cloud"
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t* remote) {
    (void)remote; /* not used */
    if (req_state == GCOAP_MEMO_PDU_RECV) {
        printf("Response received from server\n");
    }
    else {
        printf("Response error\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    // Initialize address
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    
    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error in converting IP address\n");
        return 1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("gcoap_req_send() failed with %ld\n", res);
    } else {
        printf("Request sent\n");
    }

    return 0;
}