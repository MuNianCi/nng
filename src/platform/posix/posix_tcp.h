//
// Copyright 2025 Staysail Systems, Inc. <info@staysail.tech>
// Copyright 2018 Capitar IT Group BV <info@capitar.com>
// Copyright 2018 Devolutions <info@devolutions.net>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

#ifndef PLATFORM_POSIX_TCP_H
#define PLATFORM_POSIX_TCP_H

#include "../../core/defs.h"
#include "../../core/reap.h"
#include "../../core/stream.h"

#include "posix_aio.h"

struct nni_tcp_conn {
	nng_stream      stream;
	nni_posix_pfd   pfd;
	nni_list        readq;
	nni_list        writeq;
	bool            closed;
	nni_mtx         mtx;
	nni_aio        *dial_aio;
	nni_tcp_dialer *dialer;
	nni_reap_node   reap;
	nng_sockaddr    peer;
	nng_sockaddr    self;
};

// TCP socket tuning options captured at dial/listen time and applied
// to the connection in nni_posix_tcp_start().  A value of 0 means
// "not set": that setsockopt() is never issued.
struct nni_tcp_opts {
	int to_nodelay;     // TCP_NODELAY on/off (0 or 1)
	int to_keepalive;   // SO_KEEPALIVE on/off (0 or 1)
	int to_keepidle;    // TCP_KEEPIDLE in ms (converted to sec)
	int to_keepintvl;   // TCP_KEEPINTVL in ms (converted to sec)
	int to_keepcnt;     // TCP_KEEPCNT probes
	int to_usertimeout; // TCP_USER_TIMEOUT in ms
};

extern int  nni_posix_tcp_alloc(nni_tcp_conn **, nni_tcp_dialer *, int);
extern void nni_posix_tcp_start(nni_tcp_conn *, const struct nni_tcp_opts *);
extern void nni_posix_tcp_dialer_rele(nni_tcp_dialer *);
extern void nni_posix_tcp_dial_cb(void *, unsigned);

#endif // PLATFORM_POSIX_TCP_H
