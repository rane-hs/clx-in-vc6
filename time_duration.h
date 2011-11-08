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
 *  Last-modified: Tue 15 Feb 2011 11:30:02 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CLX_TIME_DURATION_H
#define CLX_TIME_DURATION_H

namespace clx {
	
	/* --------------------------------------------------------------------- */
	//  time_duration
	/* --------------------------------------------------------------------- */
	class time_duration {
	public:
		typedef int value_type;
		
		explicit time_duration(
			const value_type d,
			const value_type h, const value_type n, const value_type s) :
			day_(d), hour_(h), minute_(n), second_(s) {}
		explicit time_duration(const time_t &t = 0)
		{
			day_ = static_cast<value_type>(t/(60*60*24));
			hour_ = static_cast<value_type>((t/(60*60))%24);
			minute_ = static_cast<value_type>((t/(60))%60);
			second_ = static_cast<value_type>(t%60);
		}
		
		time_duration& operator+=(const time_duration& t) {
			day_ += t.day_;
			hour_ += t.hour_;
			minute_ += t.minute_;
			second_ += t.second_;
			return *this;
		}
		
		time_duration& operator-=(const time_duration& t) {
			day_ -= t.day_;
			hour_ -= t.hour_;
			minute_ -= t.minute_;
			second_ -= t.second_;
			return *this;
		}

		const time_t c_time() const
		{
			return 	seconds() ;
		}
		
		// binary operator
		friend time_duration operator+(const time_duration& x, const time_duration& y) {
			return time_duration(x.days() + y.days(), x.hours() + y.hours(), x.minutes() + y.minutes(), x.seconds() + y.seconds());
		}
		friend time_duration operator-(const time_duration& x, const time_duration& y) { 
			return time_duration(x.days() - y.days(), x.hours() - y.hours(), x.minutes() - y.minutes(), x.seconds() - y.seconds());
		}

		// accessor
		const value_type days() const 
		{ return day_ + (hour_/24) + (minute_ / (60*24)) + (second_/(60*60*24));
		}
		const value_type hours() const 
		{ return (day_*24) + hour_ + (minute_ / 60) + (second_/(60*60));
		}
		const value_type minutes() const
		{ return (day_*24*60) + (hour_*60) + minute_  + (second_/60);
		}
		const value_type seconds() const
		{ return (day_*24*60*60) + (hour_*60*60) + (minute_*60)  + second_;
		}
		const value_type day() const { return day_; }
		const value_type hour() const { return hour_; }
		const value_type minute() const { return minute_; }
		const value_type second() const { return second_; }
		
	private:
		value_type day_;
		value_type hour_;
		value_type minute_;
		value_type second_;
	};
	
	/* --------------------------------------------------------------------- */
	//  time_unit helper func
	/* --------------------------------------------------------------------- */
	inline const time_duration days(int t)    { return time_duration(t, 0, 0, 0); }
	inline const time_duration hours(int t)   { return time_duration(0, t, 0, 0); }
	inline const time_duration minutes(int t) { return time_duration(0, 0, t, 0); }
	inline const time_duration seconds(int t) { return time_duration(0, 0, 0, t); }

}

#endif // CLX_TIME_DURATION_H
