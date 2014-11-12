/*
 * This file is part of the bladeRF project:
 *   http://www.github.com/nuand/bladeRF
 *
 * Copyright (C) 2013 Nuand LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#if !defined(WIN32) && !defined(__CYGWIN__) && defined(LOG_SYSLOG_ENABLED)
#include <syslog.h>
#endif
#include "log.h"
#include "bladerf_priv.h"

#if !defined(WIN32) && !defined(__CYGWIN__)
#  if !defined(__clang__) && !defined(__GNUC__)
#  error init/fini mechanism not known to work for your compiler.
#  endif
#define __init __attribute__((constructor))
#define __fini __attribute__((destructor))
#else
/* Corresponding syntax for Windows (TBD) */
#define __init
#define __fini
#endif

/* Module initializers/deinitializers. When used as library (who don't have
 * a natural entry/exit function) these are used to initialize
 * deinitialize. Use to set predefined/default states and cleanup.
 *
 * This will work with shared libraries as well as with static as they get
 * invoked by RTL load/unload, with or without C++ code (i.e. functions will
 * play nice with C++ normal ctors/dtors).
 *
 * Keep log in to at least once per new build-/run-environment assert that
 * the mechanism works.
 */

#if defined(WAIT_FOR_DEBUGGER)
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#define BREAK_HERE raise(SIGINT)
int dloop=0;

int IsDebuggerPresent(void)
{
    char buf[1024];
    int debugger_present = 0;

    int status_fd = open("/proc/self/status", O_RDONLY);
    if (status_fd == -1)
        return 0;

    ssize_t num_read = read(status_fd, buf, sizeof(buf));

    if (num_read > 0)
    {
        static const char TracerPid[] = "TracerPid:";
        char *tracer_pid = strstr(buf, TracerPid);

        if (tracer_pid)
            debugger_present = !!atoi(tracer_pid + sizeof(TracerPid) - 1);
    }

    return debugger_present;
}

#endif

void __init __bladerf_init(void) {

#if !defined(WIN32) && !defined(__CYGWIN__) && defined(LOG_SYSLOG_ENABLED)
    openlog("bladeRF",
        LOG_CONS | LOG_NDELAY  | LOG_NOWAIT | LOG_PERROR | LOG_PID, LOG_USER);
    bladerf_log_set_verbosity(BLADERF_LOG_LEVEL_INFO);
#endif

#if defined(WAIT_FOR_DEBUGGER) && WAIT_FOR_DEBUGGER>0
    bladerf_log_set_verbosity(BLADERF_LOG_LEVEL_VERBOSE);
    while (dloop<WAIT_FOR_DEBUGGER) {
        int dpid;
        if (!(dpid=IsDebuggerPresent())) {
            log_debug("Waiting for debugger to attach %d (%d)...", dloop++, dpid);
            sleep(1);
        } else {
            log_debug("Debugger attached at PID=%d)...", dpid);
            break;
        }
    }
    if (dloop!=WAIT_FOR_DEBUGGER)
        BREAK_HERE;
    bladerf_log_set_verbosity(BLADERF_LOG_LEVEL_INFO);
#endif

    log_info("BladeRF host initialized");
}

void __fini __bladerf_fini(void) {
    log_info("BladeRF host deinitialized");
    fflush(NULL);
#if !defined(WIN32) && !defined(__CYGWIN__) && defined(LOG_SYSLOG_ENABLED)
    closelog();
#endif
}
