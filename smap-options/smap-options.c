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
  	shiba_fatal("FATAL: Failed to allocate space for scan info targets! (init_scan_info)");

  s->targets->target = NULL;
  s->targets->next = NULL;
  s->num_ports = 0;
  s->closed_ports = 0;
  s->open_ports = 0;
  s->scan_type = SCAN_SYN;
  s->ignored_ports = 0;

  for (int i = 0; i < MAX_PORT + 1; i++)
    s->port_list[i] = FALSE;
}

void scan_info_cleanup(scan_info_t *s) {
	target_t* last = NULL;

	while (s->targets) {
		last = s->targets;
		s->targets = s->targets->next;
		free(last);
	}

	free(s);
	s = NULL;
}
