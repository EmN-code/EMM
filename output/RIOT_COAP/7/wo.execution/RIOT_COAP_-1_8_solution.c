#include "stdio.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "netinet/in.h"
#include "sys/socket.h"

// Mock handlers and structures due to lack of net/gcoap.h usage permission
typedef void (*gcoap_resp_handler_t)(unsigned, void *, void *);
typedef struct coap_pkt {
    uint8_t *payload;
    size_t len;
} coap_pkt_t;

// Mock function which is originally part of sock_udp
static ssize_t udp_send(const uint8_t *buf, size_t len, const struct sockaddr *dest_addr, socklen_t addrlen) {
    // Implement UDP sending logic or mock it
    printf("Mock UDP send: %s\n", buf);
    return len;
}

static void _resp_handler(unsigned x, coap_pkt_t *pdu, void *context) {
    printf("Response handler called with payload: %s\n", (char*)pdu->payload);
}

int main() {
    const char *EMQX_SERVER_IP = "47.102.103.1";
    const char *EMQX_URI_PATH = "/coap/test";
    const char *payload = "Hello, COAP Cloud";

    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons(5683);

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.sin_addr) <= 0) {
        printf("Error: Invalid IP address\n");
        return -1;
    }

    coap_pkt_t pdu;
    pdu.payload = (uint8_t *)payload;
    pdu.len = strlen(payload);

    udp_send((const uint8_t *)pdu.payload, pdu.len, (const struct sockaddr *)&remote, sizeof(remote));

    return 0;
}
