/* ------------------------------------------------------------------------- */
/*
 *  date_time.h
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
 *  Last-modified: Sun 04 Jan 2009 17:48:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CLX_DATE_TIME_H
#define CLX_DATE_TIME_H

#include "config.h"

#define CLX_TZ_LOCAL 0
#define CLX_TZ_UTC 1
#define DEFAULT_DATE_TIME_FORMAT "%Y-%m-%dT%H:%M:%S"

#include <string>
#include <ctime>
#include <stdexcept>
#include "literal.h"
#include "code_convert.h"
#include "time_duration.h"
#ifdef CLX_WIN32
#include "win32/date_time.h"
#else
#include "unix/date_time.h"
#endif

#if defined(_MSC_VER) && (_MSC_VER == 1200)
typedef time_t clx_time_t;
#else
typedef std::time_t clx_time_t;
#endif

namespace clx {

	namespace detail {
		/* ----------------------------------------------------------------- */
		//  now
		/* ----------------------------------------------------------------- */

		/*	
			VC6はテンプレートにバグがあるため、VC6でも動くように修正を行う
			参考：http://www.fides.dti.ne.jp/~oka-t/cpplab-vc6-func-template-bug.html
		*/
		template <int TimeZone>
		class now
		{
		public:
			void operator()(struct tm *result) {
#if defined(_MSC_VER) && (_MSC_VER == 1200)
				clx_time_t t = time(NULL);
#else
				clx_time_t t = std::time(NULL);
#endif
				xgmtime(&t, result);
			}
		};

		template <>
		class now<CLX_TZ_LOCAL>
		{
		public:
			void operator()(struct tm *result) {
				clx_time_t t = time(NULL);
				xlocaltime(&t, result);
			}
		};
		
		/* ----------------------------------------------------------------- */
		//  time_to_string
		/* ----------------------------------------------------------------- */
		template <class CharT>
		inline size_t time_to_string(CharT* dest, size_t n, const CharT* fmt, const struct tm* date) {
#if defined(_MSC_VER) && (_MSC_VER == 1200)
			return strftime(dest, n, fmt, date);
#else
			return std::strftime(dest, n, fmt, date);
#endif
		}
		
#ifdef CLX_USE_WCHAR
		template <>
		inline size_t time_to_string<wchar_t>(wchar_t* dest, size_t n, const wchar_t* fmt, const struct tm* date) {
			return std::wcsftime(dest, n, fmt, date);
		}
#endif // CLX_USE_WCHAR
	};
	
	/* --------------------------------------------------------------------- */
	//  basic_date_time
	/* --------------------------------------------------------------------- */
	template <int TimeZone>
	class basic_date_time {
	public:
		typedef int value_type;
		typedef struct tm tm_type;
		
		// constructor and destructor
		basic_date_time() {
			tm_type tmp;
			detail::now<TimeZone>()(&tmp);
			this->assign(tmp);
		}
		
		explicit basic_date_time(const tm_type& t) { this->assign(t); }
		explicit basic_date_time(const clx_time_t& t) { this->assign(t); }
		
		explicit basic_date_time(value_type& y, value_type& m, value_type& d,
			value_type& h, value_type& mm, value_type& s) {
			this->assign(y, m, d, h, mm, s);
		}
		
		basic_date_time& operator=(const basic_date_time& t) { return this->assign(t); }
		basic_date_time& operator=(const tm_type& t) { return this->assign(t); }
		basic_date_time& operator=(const clx_time_t& t) { return this->assign(t); }
		
		basic_date_time& operator+=(value_type t) { return this->after(t); }
		basic_date_time& operator+=(const time_duration& t) { return this->after(t); }
		basic_date_time& operator-=(value_type t) { return this->before(t); }
		basic_date_time& operator-=(const time_duration& t) { return this->before(t); }
		
		time_duration operator-(const basic_date_time& t) const { return this->difference(t); }
		time_duration operator-(const clx_time_t& t) const { return this->difference(t); }
		time_duration operator-(const tm_type& t) const { return this->difference(t); }
		
		// binary operator
		friend basic_date_time operator+(basic_date_time x, value_type y) { return x += y; }
		friend basic_date_time operator+(basic_date_time x, const time_duration& y) { return x += y; }
		friend basic_date_time operator-(basic_date_time x, value_type y) { return x -= y; }
		friend basic_date_time operator-(basic_date_time x, const time_duration& y) { return x -= y; }
		friend basic_date_time operator+(value_type y, basic_date_time x) { return x += y; }
		friend basic_date_time operator+(const time_duration& y, basic_date_time x) { return x += y; }
		friend basic_date_time operator-(value_type y, basic_date_time x) { return x -= y; }
		friend basic_date_time operator-(const time_duration& y, basic_date_time x) { return x -= y; }
		
		// comparison operator
		friend bool operator<(const basic_date_time& x, const basic_date_time& y) {
#if defined(_MSC_VER) && (_MSC_VER == 1200)
			if (difftime(y.c_time(), x.c_time()) > 0.01) return true;
#else
			if (std::difftime(y.c_time(), x.c_time()) > 0.01) return true;
#endif
			return false;
		}
		
		friend bool operator>(const basic_date_time& x, const basic_date_time& y) {
#if defined(_MSC_VER) && (_MSC_VER == 1200)
			if (difftime(x.c_time(), y.c_time()) > 0.01) return true;
#else
			if (std::difftime(x.c_time(), y.c_time()) > 0.01) return true;
#endif
			return false;
		}
		
		friend bool operator<=(const basic_date_time& x, const basic_date_time& y) {
			return !(x > y);
		}
		
		friend bool operator>=(const basic_date_time& x, const basic_date_time& y) {
			return !(x < y);
		}
		
		friend bool operator==(const basic_date_time& x, const basic_date_time& y) {
			if (!(x < y) && !(x > y)) return true;
			return false;
		}
		
		friend bool operator!=(const basic_date_time& x, const basic_date_time& y) {
			return !(x == y);
		}
		
		// accessor
		value_type year() const { return date_.tm_year + 1900; }
		value_type month() const { return date_.tm_mon + 1; }
		value_type day() const { return date_.tm_mday; }
		value_type hour() const { return date_.tm_hour; }
		value_type minute() const { return date_.tm_min; }
		value_type second() const { return date_.tm_sec; }
		value_type totaldays() const { return date_.tm_yday; }
		value_type weekday() const { return date_.tm_wday; }
		
		// get information as the traditional std::tm type
		const tm_type& reference() const { return date_; }
		const tm_type* data() const { return &date_; }
		
		clx_time_t c_time() const {
			tm_type tmp = date_;
#if defined(_MSC_VER) && (_MSC_VER == 1200)
			return mktime(&tmp);
#else
			return std::mktime(&tmp);
#endif
		}
		
		void reset() {
			tm_type tmp;
			detail::now<TimeZone>()(&tmp);
			this->assign(tmp);
		}
		
		basic_date_time& assign(const basic_date_time& t) {
			return this->assign(t.date_);
		}
		
		basic_date_time& assign(const tm_type& t) {
			date_ = t;
			return *this;
		}
		
		basic_date_time& assign(const clx_time_t& t) {
			struct tm tval;
			detail::xlocaltime(&t, &tval);
			return this->assign(tval);
		}
		
		basic_date_time& assign(value_type y, value_type m, value_type d,
			value_type h, value_type mm, value_type s) {
			date_.tm_year = y - 1900;
			date_.tm_mon  = m - 1;
			date_.tm_mday = d;
			date_.tm_hour = h;
			date_.tm_min  = mm;
			date_.tm_sec  = s;
			return *this;
		}
		
		basic_date_time& after(value_type t) {
			return this->assign(this->c_time() + t);
		}
		
		basic_date_time& after(const time_duration& t) {
			return this->assign(c_time() + t.c_time());
		}
		
		basic_date_time& before(value_type t) {
			clx_time_t tmp = this->c_time() - t;
			if (tmp < 0) throw std::range_error("negative value");
			return this->assign(tmp);
		}
		
		basic_date_time& before(const time_duration& t) {
			clx_time_t tmp = this->c_time() - t.c_time();
			
			return this->assign(tmp);
		}
		
		time_duration difference(const basic_date_time& t) const {
			return time_duration(static_cast<time_t>(std::difftime(this->c_time(), t.c_time())));
		}
		
		time_duration difference(const clx_time_t& t) const {
			return time_duration(static_cast<time_t>(std::difftime(this->c_time(), t)));
		}
		
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
		time_duration difference(const std::tm& t) const {
			clx_time_t tmp = std::mktime(const_cast<std::tm*>(&t));
			return time_duration(static_cast<time_t>(std::difftime(this->c_time(), tmp)));
		}
#else
		time_duration difference(const tm& t) const {
			clx_time_t tmp = std::mktime(const_cast<tm*>(&t));
			return std::difftime(this->c_time(), tmp);
		}
#endif
		
		// string conversion
		template <class CharT>
		basic_date_time& from_string(const CharT* s, const CharT* fmt) {
			detail::string_to_time(s, fmt, &date_);
			return *this;
		}
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
		template <class CharT>
		basic_date_time& from_string(const std::basic_string<CharT>& s,
			const std::basic_string<CharT>& fmt = LITERAL("%Y-%m-%dT%H:%M:%S")) {
			return this->from_string(s.c_str(), fmt.c_str());
		}
#endif
		
		
		template <class CharT>
		std::basic_string<CharT> to_string(const CharT* fmt) const {
			CharT tmp[1024];
			size_t n = detail::time_to_string(tmp, sizeof(tmp) / sizeof(CharT), fmt, &date_);
			if (n == 0) throw std::runtime_error("temporary buffer is overflow");
			return std::basic_string<CharT>(tmp);
		}
		
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
		template <class CharT>
		std::basic_string<CharT> to_string(const std::basic_string<CharT>& fmt = LITERAL(DEFAULT_DATE_TIME_FORMAT)) const {
			return this->to_string(fmt.c_str());
		}
#endif
		
	private:
		tm_type date_;
	};
	
	typedef basic_date_time<CLX_TZ_LOCAL> date_time;
	typedef basic_date_time<CLX_TZ_UTC> date_time_utc;
}

#endif // CLX_DATE_TIME_H
