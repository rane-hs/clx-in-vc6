/* ------------------------------------------------------------------------- */
/*
 *  time_duration.h
 *
 *  Copyright (c) 2004 - 2008, clown. All rights reserved.
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
 *  Last-modified: Thu 14 Jun 2007 14:38:02 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CLX_TIME_DURATION_H
#define CLX_TIME_DURATION_H

#include "unit.h"

namespace clx {
	
	/* --------------------------------------------------------------------- */
	//  time_duration
	/* --------------------------------------------------------------------- */
	class time_duration {
	public:
		typedef int value_type;
		
		explicit time_duration(
			const value_type y = 0, const value_type m = 0, const value_type d = 0,
			const value_type h = 0, const value_type n = 0, const value_type s = 0) :
			years_(y), months_(m), days_(d), hours_(h), minutes_(n), seconds_(s) {}
		
		time_duration& operator+=(const time_duration& t) {
			years_ += t.years_;
			months_ += t.months_;
			days_ += t.days_;
			hours_ += t.hours_;
			minutes_ += t.minutes_;
			seconds_ += t.seconds_;
			return *this;
		}
		
		time_duration& operator-=(const time_duration& t) {
			years_ -= t.years_;
			months_ -= t.months_;
			days_ -= t.days_;
			hours_ -= t.hours_;
			minutes_ -= t.minutes_;
			seconds_ -= t.seconds_;
			return *this;
		}
		
		// binary operator
		friend time_duration operator+(time_duration x, const time_duration& y) { return x += y; }
		friend time_duration operator-(time_duration x, const time_duration& y) { return x -= y; }
		friend time_duration operator+(const time_duration& y, time_duration x) { return x += y; }
		friend time_duration operator-(const time_duration& y, time_duration x) { return x -= y; }
		
		// accessor
		const value_type years() const { return years_; }
		const value_type months() const { return months_; }
		const value_type days() const { return days_; }
		const value_type hours() const { return hours_; }
		const value_type minutes() const { return minutes_; }
		const value_type seconds() const { return seconds_; }
		
	private:
		value_type years_;
		value_type months_;
		value_type days_;
		value_type hours_;
		value_type minutes_;
		value_type seconds_;
	};
	
	/* --------------------------------------------------------------------- */
	//  time_unit helper func
	/* --------------------------------------------------------------------- */
	inline const time_duration years(int t)   { return time_duration(t); }
	inline const time_duration months(int t)  { return time_duration(0, t); }
	inline const time_duration days(int t)    { return time_duration(0, 0, t); }
	inline const time_duration hours(int t)   { return time_duration(0, 0, 0, t); }
	inline const time_duration minutes(int t) { return time_duration(0, 0, 0, 0, t); }
	inline const time_duration seconds(int t) { return time_duration(0, 0, 0, 0, 0, t); }
}

#endif // CLX_TIME_DURATION_H
