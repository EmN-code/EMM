#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <mutex.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <kernel_defines.h>
#include <dht_params.h>
#include <unistd.h>

#define SERVER_IP           "47.102.103.1"
#define SERVER_PORT         5683
#define COAP_METHOD_POST    0x02
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    int res;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Error: Invalid IP address");
        return -1;
    }

    coap_pkt_t pdu;
    ssize_t bytes_sent;

    res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    if (res < 0) {
        puts("Error: Unable to initialize request");
        return -1;
    }

    coap_opt_add_payload_marker(&pdu);
    const char *msg = "Hello, COAP Cloud";
    memcpy(coap_payload(&pdu), msg, strlen(msg));
    pdu.payload_len = strlen(msg);

    while (true) {
        bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (bytes_sent > 0) {
            puts("Message sent");
            break;
        } else {
            puts("Error in sending, retrying...");
            sleep(1);
        }
    }

    return 0;
}