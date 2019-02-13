/*
MIT License

Copyright (c) 2019 MaticVrtacnik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer{
private:	
	using TimePoint = std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration>;
	using TimeDuration = std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period>;
	TimePoint m_startTime, m_endTime;
	TimeDuration m_duration;

public:
	Timer(){ }

	static TimePoint getCurrentTime(){ 
		return std::chrono::high_resolution_clock::now(); 
	}

	void startTimer(){
		m_startTime = this->getCurrentTime();
	}

	const Timer &endTimer(){
		m_endTime = this->getCurrentTime();
		m_duration = m_endTime - m_startTime;
		return (*this);
	}

	unsigned short getHours() const { 
		return std::chrono::duration_cast<std::chrono::hours>(m_duration).count();
	}
	unsigned short getMinutes() const { 
		return std::chrono::duration_cast<std::chrono::minutes>(m_duration).count();
	}
	unsigned int getSeconds() const { 
		return std::chrono::duration_cast<std::chrono::seconds>(m_duration).count();
	}
	unsigned long getMiliseconds() const { 
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_duration).count();
	}
	unsigned long long getMicroseconds() const { 
		return std::chrono::duration_cast<std::chrono::microseconds>(m_duration).count();
	}
};


class CountdownTimer{
private:
	Timer m_timer;
	long m_duration;

public:
	CountdownTimer(long durationInMiliseconds){ //miliseconds
		m_duration = durationInMiliseconds;
		m_timer.startTimer();
	}

	bool isFinished(){
		return m_timer.endTimer().getMiliseconds() > this->m_duration;
	}
};

#endif //TIMER_H