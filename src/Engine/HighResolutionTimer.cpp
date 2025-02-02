#include "stdafx.h"
#include "HighResolutionTimer.h"
//=============================================================================
#define WINDOWS_CLOCK 0
#define HIGH_RESOLUTION_CLOCK 1
#define STEADY_CLOCK 2
//=============================================================================
#define USE_CLOCK WINDOWS_CLOCK
//=============================================================================
#if USE_CLOCK == WINDOWS_CLOCK
class HighResolutionTimerImpl final
{
public:
	HighResolutionTimerImpl()
	{
		QueryPerformanceFrequency(&m_frequency);
		QueryPerformanceCounter(&m_t0);
	}

	void Tick()
	{
		QueryPerformanceCounter(&m_t1);
		// Compute the value in microseconds (1 second = 1,000,000 microseconds)
		m_elapsedTime = (m_t1.QuadPart - m_t0.QuadPart) * (1000000.0 / m_frequency.QuadPart);

		m_t0 = m_t1;
	}

	auto GetElapsedTimeInMicroSeconds() { return m_elapsedTime; }

private:
	LARGE_INTEGER m_t0, m_t1;
	LARGE_INTEGER m_frequency;
	double        m_elapsedTime{ 0.0 };
};
#elif USE_CLOCK == HIGH_RESOLUTION_CLOCK
class HighResolutionTimerImpl final
{
public:
	HighResolutionTimerImpl()
	{
		m_t0 = std::chrono::high_resolution_clock::now();
	}

	void Tick()
	{
		m_t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::micro> delta = m_t1 - m_t0;
		m_t0 = m_t1;
		m_elapsedTime = delta.count();
	}

	auto GetElapsedTimeInMicroSeconds() { return m_elapsedTime; }

private:
	std::chrono::high_resolution_clock::time_point m_t0, m_t1;
	double                                         m_elapsedTime{ 0.0 };
};
#elif USE_CLOCK == STEADY_CLOCK
class HighResolutionTimerImpl final
{
public:
	HighResolutionTimerImpl()
	{
		m_t0 = steady_clock::now();
	}

	void Tick()
	{
		m_t1 = std::chrono::steady_clock::now();
		std::chrono::duration<double, std::micro> delta = m_t1 - m_t0;
		m_t0 = m_t1;
		m_elapsedTime = delta.count();
	}

	auto GetElapsedTimeInMicroSeconds() { return m_elapsedTime; }

private:
	std::chrono::steady_clock::time_point m_t0, m_t1;
	double                                m_elapsedTime{ 0.0 };
};
#endif
//=============================================================================
HighResolutionTimer::HighResolutionTimer(void)
{
	m_impl = new HighResolutionTimerImpl();
}
//=============================================================================
HighResolutionTimer::~HighResolutionTimer(void)
{
	delete m_impl;
}
//=============================================================================
void HighResolutionTimer::Tick()
{
	m_impl->Tick();
}
//=============================================================================
double HighResolutionTimer::ElapsedSeconds() const
{
	return m_impl->GetElapsedTimeInMicroSeconds() * 0.000001;
}
//=============================================================================
double HighResolutionTimer::ElapsedMilliSeconds() const
{
	return m_impl->GetElapsedTimeInMicroSeconds() * 0.001;
}
//=============================================================================
double HighResolutionTimer::ElapsedMicroSeconds() const
{
	return m_impl->GetElapsedTimeInMicroSeconds();
}
//=============================================================================