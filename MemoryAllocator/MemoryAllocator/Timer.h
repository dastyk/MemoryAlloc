#ifndef _TIMER_H
#define _TIMER_H

//Timer taken from: https://www.daniweb.com/programming/software-development/code/445750/simple-timer-using-c-11-s-chrono-library

#include <chrono>
#include <string>
#include <fstream>
class Timer {
	typedef std::chrono::high_resolution_clock high_resolution_clock;
	typedef std::chrono::milliseconds milliseconds;
public:
	explicit Timer(bool run = false)
	{
		if (run)
			Reset();
	}
	void Reset()
	{
		_start = high_resolution_clock::now();
	}
	microseconds Elapsed() const
	{
		return std::chrono::duration_cast<microseconds>(high_resolution_clock::now() - _start);
	}
	template <typename T, typename Traits>
	friend std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits>& out, const Timer& timer)
	{
		return out << timer.Elapsed().count();
	}
private:
	high_resolution_clock::time_point _start;
};



#endif
