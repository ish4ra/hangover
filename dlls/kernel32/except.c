/*
 * Copyright 2017 André Hentschel
 * Copyright 2017 Stefan Dösinger for CodeWeavers
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* NOTE: The guest side uses mingw's headers. The host side uses Wine's headers. */

#include <windows.h>
#include <stdio.h>

#include "windows-user-services.h"
#include "dll_list.h"
#include "kernel32.h"

#ifndef QEMU_DLL_GUEST
#include <wine/debug.h>
WINE_DEFAULT_DEBUG_CHANNEL(qemu_kernel32);
#endif


struct qemu_RaiseException
{
    struct qemu_syscall super;
    uint64_t code;
    uint64_t flags;
    uint64_t nbargs;
    uint64_t args;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI void WINAPI RaiseException(DWORD code, DWORD flags, DWORD nbargs, const ULONG_PTR *args)
{
    struct qemu_RaiseException call;
    call.super.id = QEMU_SYSCALL_ID(CALL_RAISEEXCEPTION);
    call.code = (uint64_t)code;
    call.flags = (uint64_t)flags;
    call.nbargs = (uint64_t)nbargs;
    call.args = (uint64_t)args;

    qemu_syscall(&call.super);
}

#else

void qemu_RaiseException(struct qemu_syscall *call)
{
    struct qemu_RaiseException *c = (struct qemu_RaiseException *)call;
    WINE_FIXME("Unverified!\n");
    RaiseException(c->code, c->flags, c->nbargs, QEMU_G2H(c->args));
}

#endif

struct qemu_UnhandledExceptionFilter
{
    struct qemu_syscall super;
    uint64_t epointers;
    uint64_t filter;
};

#ifdef QEMU_DLL_GUEST

static PTOP_LEVEL_EXCEPTION_FILTER top_filter;

LONG WINAPI kernel32_UnhandledExceptionFilter(PEXCEPTION_POINTERS epointers)
{
    struct qemu_UnhandledExceptionFilter call;

    /* For logging. */
    call.super.id = QEMU_SYSCALL_ID(CALL_UNHANDLEDEXCEPTIONFILTER);
    call.epointers = (uint64_t)epointers;
    call.filter = (uint64_t)top_filter;
    qemu_syscall(&call.super);

    if (top_filter)
    {
        LONG ret = top_filter(epointers);
        if (ret != EXCEPTION_CONTINUE_SEARCH) return ret;
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

#else

void qemu_UnhandledExceptionFilter(struct qemu_syscall *call)
{
    struct qemu_UnhandledExceptionFilter *c = (struct qemu_UnhandledExceptionFilter *)call;
    WINE_TRACE("Application filter %p.\n", QEMU_G2H(c->filter));
}

#endif

struct qemu_SetUnhandledExceptionFilter
{
    struct qemu_syscall super;
    uint64_t filter;
    uint64_t old;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI LPTOP_LEVEL_EXCEPTION_FILTER WINAPI SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER filter)
{
    struct qemu_SetUnhandledExceptionFilter call;
    LPTOP_LEVEL_EXCEPTION_FILTER old = top_filter;

    call.super.id = QEMU_SYSCALL_ID(CALL_SETUNHANDLEDEXCEPTIONFILTER);
    call.filter = (uint64_t)filter;
    call.old = (uint64_t)old;

    /* For logging. */
    qemu_syscall(&call.super);

    top_filter = filter;
    return old;
}

#else

void qemu_SetUnhandledExceptionFilter(struct qemu_syscall *call)
{
    struct qemu_SetUnhandledExceptionFilter *c = (struct qemu_SetUnhandledExceptionFilter *)call;
    WINE_TRACE("Setting filter %p, old %p.\n", QEMU_G2H(c->filter), QEMU_G2H(c->old));
}

#endif

struct qemu_FatalAppExitA
{
    struct qemu_syscall super;
    uint64_t action;
    uint64_t str;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI void WINAPI FatalAppExitA(UINT action, LPCSTR str)
{
    struct qemu_FatalAppExitA call;
    call.super.id = QEMU_SYSCALL_ID(CALL_FATALAPPEXITA);
    call.action = (uint64_t)action;
    call.str = (uint64_t)str;

    qemu_syscall(&call.super);
}

#else

void qemu_FatalAppExitA(struct qemu_syscall *call)
{
    struct qemu_FatalAppExitA *c = (struct qemu_FatalAppExitA *)call;
    WINE_FIXME("Unverified!\n");
    FatalAppExitA(c->action, QEMU_G2H(c->str));
}

#endif

struct qemu_FatalAppExitW
{
    struct qemu_syscall super;
    uint64_t action;
    uint64_t str;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI void WINAPI FatalAppExitW(UINT action, LPCWSTR str)
{
    struct qemu_FatalAppExitW call;
    call.super.id = QEMU_SYSCALL_ID(CALL_FATALAPPEXITW);
    call.action = (uint64_t)action;
    call.str = (uint64_t)str;

    qemu_syscall(&call.super);
}

#else

void qemu_FatalAppExitW(struct qemu_syscall *call)
{
    struct qemu_FatalAppExitW *c = (struct qemu_FatalAppExitW *)call;
    WINE_FIXME("Unverified!\n");
    FatalAppExitW(c->action, QEMU_G2H(c->str));
}

#endif

struct qemu_FatalExit
{
    struct qemu_syscall super;
    uint64_t ExitCode;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI void WINAPI FatalExit(int ExitCode)
{
    struct qemu_FatalExit call;
    call.super.id = QEMU_SYSCALL_ID(CALL_FATALEXIT);
    call.ExitCode = (uint64_t)ExitCode;

    qemu_syscall(&call.super);
}

#else

void qemu_FatalExit(struct qemu_syscall *call)
{
    struct qemu_FatalExit *c = (struct qemu_FatalExit *)call;
    WINE_FIXME("Unverified!\n");
    FatalExit(c->ExitCode);
}

#endif
