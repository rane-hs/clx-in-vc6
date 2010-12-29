/* ------------------------------------------------------------------------- */
/*
 *  mpl/bitmask.h
 *
 *  Copyright (c) 2004 - 2010, clown. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    - No names of its contributors may be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  Last-modified: Thu 28 Jan 2010 04:22:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CLX_MPL_BITMASK_H
#define CLX_MPL_BITMASK_H

#include <cstddef>
#include <climits>

namespace clx {
	namespace mpl {
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
		typedef size_t size_type;
#else
		typedef std::size_t size_type;
#endif
		/* ----------------------------------------------------------------- */
		//  lower_mask
		/* ----------------------------------------------------------------- */
		template <size_type Bits, typename T = size_type>
		struct lower_mask {
			typedef T type;
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
			static const type value(){ return (type(1) << Bits) - 1; }
#else
			static const type value = (type(1) << Bits) - 1;
#endif
		};
		
		/* ----------------------------------------------------------------- */
		//  upper_mask
		/* ----------------------------------------------------------------- */
		template <size_type Bits, typename T = size_type>
		struct upper_mask {
			typedef T type;
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
			static const type value(){ return (~lower_mask<Bits, T>::value()); }
#else
			static const type value = ~lower_mask<Bits, T>::value;
#endif
		};
	} // namespace mpl
} // namespace clx

#endif // CLX_MPL_BITMASK_H