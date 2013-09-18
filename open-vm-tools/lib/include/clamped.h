/*********************************************************
 * Copyright (C) 2009 VMware, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation version 2.1 and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the Lesser GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.
 *
 *********************************************************/

/*********************************************************
 * The contents of this file are subject to the terms of the Common
 * Development and Distribution License (the "License") version 1.0
 * and no later version.  You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at
 *         http://www.opensource.org/licenses/cddl1.php
 *
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 *********************************************************/

/*
 * clamped.h --
 *
 *      Clamped arithmetic. This header file provides inline
 *      arithmetic operations that don't overflow. Instead, they
 *      saturate at the data type's max or min value.
 */

#ifndef _CLAMPED_H_
#define _CLAMPED_H_

#include "vm_basic_types.h"


/*
 *----------------------------------------------------------------------
 *
 * Clamped_SAdd32 --
 *
 *      Signed 32-bit addition.
 *
 *      Add two integers, clamping the result to MAX_INT32 or
 *      MIN_INT32 if it would have overflowed.
 *
 * Results:
 *      On success, returns TRUE.
 *      If the result would have overflowed and we clamped it, returns FALSE.
 *
 * Side effects:
 *      None.
 *
 *----------------------------------------------------------------------
 */

static INLINE Bool
Clamped_SAdd32(int32 *out,  // OUT
               int32 a,     // IN
               int32 b)     // IN
{
   int32 result;

   result = a + b;

   if (UNLIKELY(b > 0 && result < a)) {
      *out = MAX_INT32;
      return FALSE;
   }

   if (UNLIKELY(b < 0 && result > a)) {
      *out = MIN_INT32;
      return FALSE;
   }

   *out = result;
   return TRUE;
}


/*
 *-----------------------------------------------------------------------------
 *
 * Clamped_U64To32 --
 *
 *      Convert Unsigned 64-bit to 32-bit, clamping instead of truncating.
 *
 * Results:
 *      On success, returns TRUE. If the result would have overflowed
 *      and we clamped it to MAX_UINT32, returns FALSE.
 *
 * Side effects:
 *      None.
 *
 *-----------------------------------------------------------------------------
 */

static INLINE Bool
Clamped_U64To32(uint32 *out,  // OUT
                uint64 a)     // IN
{
   uint32 clamped = (uint32)a;

   if (UNLIKELY(a != clamped)) {
      *out = MAX_UINT32;
      return FALSE;
   }

   *out = clamped;
   return TRUE;
}


/*
 *-----------------------------------------------------------------------------
 *
 * Clamped_S64To32 --
 *
 *      Convert signed 64-bit to 32-bit, clamping instead of truncating.
 *
 * Results:
 *      On success, returns TRUE. If the result would have overflowed
 *      and we clamped it to MAX_INT32 or MIN_INT32, returns FALSE.
 *
 * Side effects:
 *      None.
 *
 *-----------------------------------------------------------------------------
 */

static INLINE Bool
Clamped_S64To32(int32 *out,  // OUT
                int64 a)     // IN
{
   int32 clamped = (int32)a;

   if (UNLIKELY(a != clamped)) {
      *out = a < 0 ? MIN_INT32 : MAX_INT32;
      return FALSE;
   }

   *out = clamped;
   return TRUE;
}


/*
 *-----------------------------------------------------------------------------
 *
 * Clamped_UMul32 --
 *
 *      Unsigned 32-bit multiplication.
 *
 *      We're abusing the fact that 32x32-bit multiplication always
 *      returns a 64-bit result on x86 anyway, and that the compiler
 *      should be smart enough to optimize the code here into a
 *      32x32-bit multiply.
 *
 * Results:
 *      On success, returns TRUE. If the result would have overflowed
 *      and we clamped it to MAX_UINT32, returns FALSE.
 *
 * Side effects:
 *      None.
 *
 *-----------------------------------------------------------------------------
 */

static INLINE Bool
Clamped_UMul32(uint32 *out,  // OUT
               uint32 a,     // IN
               uint32 b)     // IN
{
   return Clamped_U64To32(out, (uint64)a * b);
}


/*
 *-----------------------------------------------------------------------------
 *
 * Clamped_SMul32 --
 *
 *      Signed 32-bit multiplication.
 *
 * Results:
 *      On success, returns TRUE. If the result would have overflowed
 *      and we clamped it to MAX_INT32 or MIN_INT32, returns FALSE.
 *
 * Side effects:
 *      None.
 *
 *-----------------------------------------------------------------------------
 */

static INLINE Bool
Clamped_SMul32(int32 *out,  // OUT
               int32 a,     // IN
               int32 b)     // IN
{
   return Clamped_S64To32(out, (int64)a * b);
}


/*
 *-----------------------------------------------------------------------------
 *
 * Clamped_UAdd32 --
 *
 *      Unsigned 32-bit addition.
 *
 *      This is a utility function for 32-bit unsigned addition,
 *      in which the result is clamped to MAX_UINT32 on overflow.
 *
 * Results:
 *      On success, returns TRUE. If the result would have overflowed
 *      and we clamped it to MAX_UINT32, returns FALSE.
 *
 * Side effects:
 *      None.
 *
 *-----------------------------------------------------------------------------
 */

static INLINE Bool
Clamped_UAdd32(uint32 *out,  // OUT
               uint32 a,     // IN
               uint32 b)     // IN
{
   uint32 c = a + b;

   if (UNLIKELY(c < a || c < b)) {
      *out = MAX_UINT32;
      return FALSE;
   }

   *out = c;
   return TRUE;
}


#endif // ifndef _CLAMPED_H_