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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

int main(void);

int main()
{
   int s;
   int optval;
   socklen_t optlen = sizeof(optval);

   if((s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("socket()");
      exit(EXIT_FAILURE);
   }

   if(getsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
      perror("getsockopt()");
      close(s);
      exit(EXIT_FAILURE);
   }
   printf("SO_KEEPALIVE is %s\n", (optval ? "ON" : "OFF"));

   if(optval) {
#ifdef TCP_KEEPCNT
      if(getsockopt(s, SOL_TCP, TCP_KEEPCNT, &optval, &optlen) < 0) {
         perror("getsockopt()");
         close(s);
         exit(EXIT_FAILURE);
      }
      printf("TCP_KEEPCNT   = %d\n", optval);
#endif
   
#ifdef TCP_KEEPIDLE
      if(getsockopt(s, SOL_TCP, TCP_KEEPIDLE, &optval, &optlen) < 0) {
         perror("getsockopt()");
         close(s);
         exit(EXIT_FAILURE);
      }
      printf("TCP_KEEPIDLE  = %d\n", optval);
#endif
   
#ifdef TCP_KEEPINTVL
      if(getsockopt(s, SOL_TCP, TCP_KEEPINTVL, &optval, &optlen) < 0) {
         perror("getsockopt()");
         close(s);
         exit(EXIT_FAILURE);
      }
      printf("TCP_KEEPINTVL = %d\n", optval);
#endif
   }
   
   close(s);

   exit(EXIT_SUCCESS);
}
