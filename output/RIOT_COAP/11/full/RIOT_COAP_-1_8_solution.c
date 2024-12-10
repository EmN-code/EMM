#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <dht.h>
#include <dht_params.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/af.h>
#include <netinet/in.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <msg.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <stdio.h>
#include <mbox.h>
#include <arpa/inet.h>

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    size_t len;
    int result;

    const char *EMQX_SERVER_IP = "47.102.103.1";
    const char *PAYLOAD = "Hello, COAP Cloud";
    const char *EMQX_URI_PATH = "/coap/test";
    
    // Initialize remote endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;

    while (1) {
        printf("Attempting to resolve the IP address.\n");
        result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
        if (result != 1) {
            printf("Failed to convert IP address. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("IP address resolved successfully.\n");
            break;
        }
    }

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send the request
    ssize_t resp_status = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (resp_status <= 0) {
        printf("Failed to send CoAP request.\n");
        return -1;
    }

    printf("CoAP request sent successfully.\n");
    return 0;
}

// Placeholder for response handler function
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received from the server.\n");
}