#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <unistd.h>
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "arpa/inet.h"

define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Handler function can be used to manage responses */
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t bytes_sent;
    kernel_pid_t sender_pid = thread_getpid();
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    const char *message = "Hello, COAP Cloud";
    coap_pkt_t pdu;

    /* Convert IP address from text to binary form */
    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        fprintf(stderr, "Error: Unable to parse IP address\n");
        return -1;
    }

    /* Initialize CoAP request */
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test") == 0) {
        fprintf(stderr, "Error: Unable to initialize CoAP request\n");
        return -1;
    }

    /* Set the payload */
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, message, strlen(message));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    while (1) {
        /* Send CoAP request */
        bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (bytes_sent <= 0) {
            fprintf(stderr, "Error: Unable to send CoAP request\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);  /* Wait for 1 second before retrying */
            continue;
        }
        printf("CoAP request successfully sent\n");
        break;
    }

    return 0;
}