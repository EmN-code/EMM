#include <arpa/inet.h>
#include <mutex.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <esp_system.h>
#include <mma8x5x_regs.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 2
#define EMQX_URI_PATH "/coap/test"

void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_PENDING) {
        puts("Pending");
    } else if (req_state == GCOAP_MEMO_COMPLETE) {
        printf("Response received: %s\n", (char*)pdu->payload);
    } else {
        puts("Error");
    }
}

int main(void) {
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = 5683
    };

    inet_pton(AF_INET, "47.102.103.1", &remote.addr);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    
    const char* payload = "Hello, COAP Cloud";
    len += coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, payload, strlen(payload));
    coap_opt_finish(&pdu, len);  // corrected function call from coap_opt_set_length

    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}