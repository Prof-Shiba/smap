/*
  host-alive contains functions which will make use of ICMP to probe
  whether a host is alive or down. This is used prior to initiating
  a full port scan so we can skip wasting time on dead/non-existent hosts
*/

#pragma once

#include "./scan-engine.h"


