/*
  host-alive contains functions which will make use of ICMP to probe
  whether a host is alive or down. This is used prior to initiating
  a full port scan so we can skip wasting time on dead/non-existent hosts
*/

// TODO: heres what we can do:
// non root-users can use a TCP connect to port 80 or 443 to see if host is alive
// root users have more options. ICMP echo requests, ACK to port 80 for example,
// SYN to port 443, an ICMP timestamp request, udp, and more. We will focus on implementing these.

#pragma once

#include "./scan-engine.h"

// https://www.rfc-editor.org/rfc/rfc1071 ICMP checksum here
