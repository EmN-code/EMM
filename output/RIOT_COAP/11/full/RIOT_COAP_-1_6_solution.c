#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time_units.h>
#include <timex.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/af.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <ipv6_addr_from_str.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>

define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Buffer size for CoAP payloads

typedef void (*gcoap_resp_handler_t)(const char *arg1, const char *arg2, const char *arg3);

typedef enum {
    GCOAP_SOCKET_TYPE_UDP  // Use UDP for CoAP
} gcoap_socket_type_t;

typedef struct {
    sa_family_t family;
    union {
        struct in6_addr ipv6;
        struct in_addr ipv4;
    } addr;
    uint16_t port;
} sock_udp_ep_t;

int inet_pton(int af, const char *src, void *dst);

int main() {
    const char *COAP_SERVER_IP = "47.102.103.1";
    const char *EMQX_URI_PATH = "/coap/test";
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    sock_udp_ep_t remote;
    int rc;

    remote.family = AF_INET;
    remote.port = htons(5683);
    rc = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    if (rc != 1) {
        fprintf(stderr, "inet_pton failed\n");
        return 1;
    }

    while (1) {
        rc = gcoap_req_init((coap_pkt_t *)buf, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
        if (rc < 0) {
            fprintf(stderr, "gcoap_req_init failed\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        len = sizeof(buf);  // For example, populate buffer here

        rc = gcoap_req_send(buf, len, &remote, (gcoap_resp_handler_t)_resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc < 1) {
            fprintf(stderr, "gcoap_req_send failed\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        } else {
            fprintf(stdout, "Request sent successfully\n");
            break;
        }
    }

    return 0;
}