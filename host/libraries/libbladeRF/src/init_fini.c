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
#if defined(__linux__)
#include <syslog.h>
#endif

#define __init __attribute__((constructor))
#define __fini __attribute__((destructor))

/* Module initializers. Especially when used as library which doesn't have a
 * natural entry function. Use to set predefined/default states and cleanup.
 */
void __init __bladerf_init(void) {
#if !defined(WIN32) && !defined(__CYGWIN__)
    openlog("bladeRF",
        LOG_CONS | LOG_NDELAY  | LOG_NOWAIT | LOG_PERROR | LOG_PID, LOG_USER);
#endif
}

void __fini __bladerf_fini(void) {
}
