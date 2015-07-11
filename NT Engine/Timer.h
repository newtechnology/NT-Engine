#ifndef _TIMER_H_
#define _TIMER_H_

#include "BasicIncludes.h"

namespace NTEngine
{
	class GameTimer
	{
	public:
		DLLEXPORT GameTimer();

		DLLEXPORT inline float TotalTime()const;  // in seconds
		DLLEXPORT inline float DeltaTime()const; // in seconds

		DLLEXPORT void Reset(); // Call before message loop.
		DLLEXPORT void Start(); // Call when unpaused.
		DLLEXPORT void Stop();  // Call when paused.
		DLLEXPORT void Tick();  // Call every frame.

	private:
		double m_SecondsPerCount;
		double m_DeltaTime;

		__int64 m_BaseTime;
		__int64 m_PausedTime;
		__int64 m_StopTime;
		__int64 m_PrevTime;
		__int64 m_CurrTime;

		bool m_Stopped;
	};

}


#endif