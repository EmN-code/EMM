
#include "contiki.h"
#include "sys/etimer.h"
#include "lib/random.h"
#include "dev/leds.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ip/uip-debug.h"
#include "net/ip/uip-udp-packet.h"
#include "net/ip/uip-udp-socket.h"
#include "net/ip/uip-udp-socket-address.h"
#include "net/ip/uip-udp-socket-buffer.h"
#include "net/ip/uip-udp-socket-queue.h"
#include "net/ip/uip-udp-socket-timer.h"
#include "net/ip/uip-udp-socket-timer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue.h"
#include "net/ip/uip-udp-socket-timer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queue-process-buffer-queu