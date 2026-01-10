#include "./smap-options.h"

boolean check_if_root(void) {
#ifndef _WIN32
  if (geteuid() == 0)
    return TRUE;
  return FALSE;
#else
  boolean is_elevated = FALSE;
	HANDLE hToken = NULL;
	TOKEN_ELEVATION elevation;
	DWORD dwSize;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		goto Cleanup;

	if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
		goto Cleanup;

	is_elevated = elevation.TokenIsElevated;

Cleanup:
	if (hToken)
	{
		CloseHandle(hToken);
		hToken = NULL;
	}
	return is_elevated; 
#endif
}

void init_scan_info(scan_info_t* s) {
  s->targets = malloc(sizeof(*s->targets));
  if (!s->targets)
  	shiba_fatal("FATAL: Failed to allocate space for scan info targets! (%s)", __FILE__);

  s->port_capacity = 20;

  s->port_nums = malloc(s->port_capacity * sizeof(*s->port_nums));
  if (!s->port_nums)
  	shiba_fatal("FATAL: Malloc failed for port nums! (%s)", __FILE__);

  for (int i = 0; i <= MAX_PORT; i++) {
  	s->port_list[i].port_num = 0;
  	s->port_list[i].state = PORT_UNKNOWN;
  }

  s->targets->target = NULL;
  s->targets->next = NULL;
  s->num_ports_to_scan = 0;
  s->num_targets = 0;
  s->closed_ports = 0;
  s->open_ports = 0;
  s->scan_type = SCAN_TCP; // TODO: Default to SYN later
  s->ignored_ports = 0;
  s->af = AF_INET;
  s->sock_type = SOCK_STREAM;
  s->timeout = 2000; // milliseconds
}

void scan_info_cleanup(scan_info_t *s) {
	target_t* last = NULL;

	while (s->targets) {
		last = s->targets;
		s->targets = s->targets->next;
		free(last);
	}

	free(s->port_nums);
	free(s);

	s = NULL;
}
