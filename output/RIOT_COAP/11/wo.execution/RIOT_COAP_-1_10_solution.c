#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <stdint.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <msg.h>
#include <unistd.h>
#include <mutex.h>
#include <dht.h>
#include <dht_params.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/netif.h>
#include <netinet/in.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <net/emcute.h>
#include <time_units.h>

#define SERVER_IP        "47.102.103.1"
#define SERVER_PORT      5683
#define COAP_URI_PATH    "/coap/test"
#define COAP_PAYLOAD     "Hello, COAP Cloud"

int inet_pton(int af, const char *src, void *dst);

static ssize_t _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)req_state;
    (void)remote;
    printf("Response received: %d\n", coap_get_code(pdu));
    return 0;
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse remote IP address");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request");
        return -1;
    }

    puts("CoAP request sent");

    return 0;
}