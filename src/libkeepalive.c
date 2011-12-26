/*
    _ _ _     _                         _ _
   | (_) |__ | | _____  ___ _ __   __ _| (_)_   _____
   | | | '_ \| |/ / _ \/ _ \ '_ \ / _` | | \ \ / / _ \
   | | | |_) |   <  __/  __/ |_) | (_| | | |\ V /  __/
   |_|_|_.__/|_|\_\___|\___| .__/ \__,_|_|_| \_/ \___|
                           |_|
*/

/* Fabio Busatto */

/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program (see the file COPYING included with this
  distribution); if not, write to the Free Software Foundation, Inc.,
  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef RTLD_NEXT
#  define _GNU_SOURCE
#endif
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

int socket(int domain, int type, int protocol);

int socket(int domain, int type, int protocol)
{
  int (*libc_socket)(int, int, int);
  int s, optval;
  char *env;

  *(void **)(&libc_socket) = dlsym(RTLD_NEXT, "socket");
  if(dlerror()) {
    errno = EACCES;
    return -1;
  }

  if((s = (*libc_socket)(domain, type, protocol)) != -1) {
    if((domain == PF_INET) && (type == SOCK_STREAM)) {
      if(!(env = getenv("KEEPALIVE")) || strcasecmp(env, "off")) {
        optval = 1;
      } else {
        optval = 0;
      }
      if(!(env = getenv("KEEPALIVE")) || strcasecmp(env, "skip")) {
        setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
      }
#ifdef TCP_KEEPCNT
      if((env = getenv("KEEPCNT")) && ((optval = atoi(env)) >= 0)) {
        setsockopt(s, SOL_TCP, TCP_KEEPCNT, &optval, sizeof(optval));
      }
#endif
#ifdef TCP_KEEPIDLE
      if((env = getenv("KEEPIDLE")) && ((optval = atoi(env)) >= 0)) {
        setsockopt(s, SOL_TCP, TCP_KEEPIDLE, &optval, sizeof(optval));
      }
#endif
#ifdef TCP_KEEPINTVL
      if((env = getenv("KEEPINTVL")) && ((optval = atoi(env)) >= 0)) {
        setsockopt(s, SOL_TCP, TCP_KEEPINTVL, &optval, sizeof(optval));
      }
#endif
    }
  }

   return s;
}
