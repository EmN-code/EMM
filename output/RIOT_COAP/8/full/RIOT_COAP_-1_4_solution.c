#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <time_units.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote, void *context) {
    (void)context;  // Unused parameter
    char buf[64];
    size_t len = gcoap_resp_payload(pdu, (uint8_t *)buf, sizeof(buf));
    printf("Response received from %s:%u \n", inet_ntoa(remote->port), remote->port);
    if (len) {
        printf("%.*s\n", (int)len, buf);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Convert IP address
    while (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address. Retrying...\n");
        ztimer_sleep(ZTIMER_1, 1);
    }
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    int rc;
    while (1) {
        rc = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
        if (rc < 0) {
            printf("Failed to initialize request: %d\n", rc);
            ztimer_sleep(ZTIMER_1, 1);
            continue;
        }
        rc = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc < 0) {
            printf("Failed to send request: %d\n", rc);
            ztimer_sleep(ZTIMER_1, 1);
            continue;
        }
        printf("Request sent successfully\n");
        break;
    }

    return 0;
}