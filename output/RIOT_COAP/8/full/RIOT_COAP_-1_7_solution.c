#include <ztimer.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>

#define COAP_PORT           (5683)
#define EMQX_URI_PATH       "/coap/test"
#define EMQX_SERVER_IP      "47.102.103.1"
#define COAP_MSG            "Hello, COAP Cloud"

int main(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;
    ssize_t res;

    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    while (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("IP address conversion failed\n");
        sleep(1);
    }

    while (1) {
        size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
        coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
        memcpy(pdu.payload, COAP_MSG, strlen(COAP_MSG));
        len += strlen(COAP_MSG);

        res = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res <= 0) {
            printf("Failed to send request, retrying...\n");
            sleep(1);
            continue;
        }
        else {
            printf("Request sent successfully\n");
            break;
        }
    }

    return 0;
}