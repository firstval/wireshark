/* emem.h
 * Definitions for Wireshark memory management and garbage collection
 * Ronnie Sahlberg 2005
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __EMEM_H__
#define __EMEM_H__

#include <glib.h>

#include "ws_symbol_export.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**  Initialize all the memory allocation pools described below.
 *  This function must be called once when *shark initialize to set up the
 *  required structures.
 */
WS_DLL_PUBLIC
void emem_init(void);

/* Functions for handling memory allocation and garbage collection with
 * a packet lifetime scope.
 * These functions are used to allocate memory that will only remain persistent
 * until Wireshark starts dissecting the next packet in the list.
 * Everytime Wireshark starts decoding the next packet all memory allocated
 * through these functions will be released back to the free pool.
 *
 * These functions are very fast and offer automatic garbage collection:
 * Everytime a new packet is dissected, all memory allocations done in
 * the previous packet is freed.
 */

/** Allocate memory with a packet lifetime scope */
WS_DLL_PUBLIC
void *ep_alloc(size_t size) G_GNUC_MALLOC;

/** Allocate memory with a packet lifetime scope and fill it with zeros*/
WS_DLL_PUBLIC
void* ep_alloc0(size_t size) G_GNUC_MALLOC;

/** Create a formatted string with a packet lifetime scope */
WS_DLL_PUBLIC
gchar* ep_strdup_printf(const gchar* fmt, ...)
    G_GNUC_MALLOC G_GNUC_PRINTF(1, 2);

/** release all memory allocated in the previous packet dissection */
void ep_free_all(void);

/**************************************************************
 * slab allocator
 **************************************************************/

/* G_MEM_ALIGN is not always enough: http://mail.gnome.org/archives/gtk-devel-list/2004-December/msg00091.html
 * So, we check (in configure) if we need 8-byte alignment.  (Windows
 * shouldn't need such a check until someone trys running it 32-bit on a CPU
 * with more stringent alignment requirements than i386.)
 *
 * Yes, this ignores the possibility of needing 16-byte alignment for long doubles.
 */
#if defined(NEED_8_BYTE_ALIGNMENT) && (G_MEM_ALIGN < 8)
#define WS_MEM_ALIGN 8
#else
#define WS_MEM_ALIGN G_MEM_ALIGN
#endif

/* #define DEBUG_INTENSE_CANARY_CHECKS */

/** Helper to troubleshoot ep memory corruption.
 * If compiled and the environment variable WIRESHARK_DEBUG_EP_INTENSE_CANARY exists
 * it will check the canaries and when found corrupt stop there in the hope
 * the corruptor is still there in the stack.
 * Some checkpoints are already set in packet.c in strategic points
 * before and after dissection of a frame or a dissector call.
 */

#ifdef DEBUG_INTENSE_CANARY_CHECKS
void ep_check_canary_integrity(const char* fmt, ...)
    G_GNUC_PRINTF(1, 2);
#define EP_CHECK_CANARY(args) ep_check_canary_integrity args
#else
#define EP_CHECK_CANARY(args)
#endif

/**
 * Verify that the given pointer is of ephemeral type.
 *
 * @param ptr The pointer to verify
 *
 * @return TRUE if the pointer belongs to the ephemeral pool.
 */
gboolean ep_verify_pointer(const void *ptr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* emem.h */
