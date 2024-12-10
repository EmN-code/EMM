#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time_units.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <unistd.h>
#include <esp_wifi.h>
#include <esp_system.h>
//include these for RIOT 
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <mma8x5x_params.h>

#define SERVER_IP    "47.102.103.1"
#define SERVER_PORT  5683
#define URI_PATH     "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // Unused
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("timeout\n");
        return;
    }
    if (memo->state != GCOAP_MEMO_RESP) {
        printf("gcoap: response error\n");
        return;
    }
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    
    // Convert IP
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Error: unable to parse IP\n");
        return -1;
    }
    
    // Initialize CoAP request
    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Error: unable to initialize CoAP request\n");
        return -1;
    }
    
    // Set payload
    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);
    
    while (1) {
        ssize_t result = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (result <= 0) {
            printf("Send error, trying again\n");
            sleep(1);
            continue;  // Retry
        }
        else {
            printf("Request sent successfully\n");
            break;
        }
    }
    
    return 0;
}