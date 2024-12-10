#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <esp_wifi.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <sys/socket.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("timeout for msg ID %u\n", coap_get_id(pdu));
        return;
    }
    if (pdu->hdr->code == COAP_CODE_EMPTY) {
        printf("empty payload\n");
    } else {
        printf("success: code %u\n", pdu->hdr->code);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);
    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error: failed to send request\n");
    } else {
        printf("Sent %zd bytes\n", bytes_sent);
    }

    return 0;
}