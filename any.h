/* ------------------------------------------------------------------------- */
/*
 *  any.h
 *
 *  Copyright (c) 2004 - 2009, clown. All rights reserved.
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
 *  Distributed under the Boost Software License, Version 1.0. (See
 *  accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Last-modified: Mon 09 Nov 2009 02:15:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CLX_ANY_H
#define CLX_ANY_H

#if defined(_MSC_VER) && (_MSC_VER > 1200)

#include "config.h"
#include <typeinfo>
#include <stdexcept>

namespace clx {
	namespace detail {
		class holder_base {
		public:
			virtual ~holder_base() throw() {}
			virtual holder_base* clone() const = 0;
			virtual const std::type_info& type() const = 0;
		};
		
		template <class T>
		class holder : public holder_base {
		public:
			holder(const T& value) : value_(value) {}
			virtual ~holder() throw() {}
			virtual holder_base* clone() const { return new holder(value_); }
			virtual const std::type_info& type() const { return typeid(T); }
			const T& value() const { return value_; }
			
		private:
			T value_;
		};
	}
	/* --------------------------------------------------------------------- */
	//  any
	/* --------------------------------------------------------------------- */
	class any {
	public:
		any() : p_(0) {}
		
		any(const any& cp)
			: p_(cp.p_ ? cp.p_->clone() : 0) {}
		
		template <class T>
		any(const T& cp) : p_(new detail::holder<T>(cp)) {}
		
		~any() throw() { delete p_; }
		
		any& operator=(const any& cp) {
			delete p_;
			p_ = cp.p_ ? cp.p_->clone() : 0;
			return *this;
		}
		
		template <class T>
		any& operator=(const T& cp) {
			delete p_;
			p_ = new detail::holder<T>(cp);
			return *this;
		}
		
		bool empty() const { return !p_; }
		
		const std::type_info& type() const {
			return p_ ? p_->type() : typeid(void);
		}
		
		template <class T>
		const T& cast() const {
			if (!p_) throw std::runtime_error("NULL pointer exception");
			return dynamic_cast<detail::holder<T>& >(*p_).value();
		}
		
	private:
		
		detail::holder_base* p_;
	};
	template <class T>
	inline const T any_cast(const any &val) {
		detail::holder<T> val_ = dynamic_cast<detail::holder<T>& >(val);
		return val_.value();
	}
}
#else //vc6

#include <boost/any.hpp>
namespace clx{
	using boost::any;
	using boost::any_cast;
};

#endif


#endif // CLX_ANY_H
