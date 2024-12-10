#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "msg.h"
#include "mutex.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_MESSAGE "Hello, COAP Cloud"
#define RECONNECT_DELAY (1U * US_PER_SEC)

gcoap_resp_handler_t _resp_handler = NULL;

gcoap_socket_type_t tl_type = GCOAP_SOCKET_TYPE_UDP;

int network_connect(sock_udp_ep_t *remote) {
    int res;
    while (1) {
        res = inet_pton(AF_INET, COAP_SERVER_IP, &remote->addr.ipv4);
        if (res <= 0) {
            printf("Failed to convert IP address. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        remote->port = COAP_PORT;
        break;
    }
    return res;
}

int main(void) {
    uint8_t buf[128];
    size_t len;
    int res;

    // Initialize remote
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_NETIF_ANY }; 
    
    // Connect to network
    if (network_connect(&remote) != 1) {
        printf("Network connection failed.");
        return 1;
    }

    // Initialize CoAP request
    coap_pkt_t pdu;
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    len += coap_payload(&pdu, (uint8_t *)COAP_MESSAGE, strlen(COAP_MESSAGE));

    // Send CoAP request
    while (1) {
        res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, tl_type);
        if (res <= 0) {
            printf("Failed to send CoAP request. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("CoAP request sent successfully.\n");
        break;
    }

    return 0;
}
