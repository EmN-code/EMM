
#include "contiki.h"
#include "sys/etimer.h"
#include "lib/random.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "dev/gpio-hal.h"
#include "dev/uart0.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-addr.h"
#includ