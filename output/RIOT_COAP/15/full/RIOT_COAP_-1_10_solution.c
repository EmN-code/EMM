#include <ztimer.h>
#include <xtimer.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"
#define PANIC(_rc) { printf("Error: %d\n", (_rc)); exit(EXIT_FAILURE); }

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr) != 1) {
        PANIC(errno);
    }

    char buf[128];
    coap_pkt_t pdu;
    
    size_t len = gcoap_req_init(&pdu, (uint8_t *)buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    coap_hdr_set_type((coap_hdr_t *)buf, COAP_TYPE_NON);
    coap_hdr_set_payload_len((coap_hdr_t *)buf, strlen(COAP_MESSAGE));
    len += strlen(COAP_MESSAGE);
    
    int rc;
    while (1) {
        rc = gcoap_req_send((uint8_t *)buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc < 0) {
            printf("Send failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Send succeeded\n");
            break;
        }
    }
    return 0;
}