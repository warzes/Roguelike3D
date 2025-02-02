#pragma once

class HighResolutionTimerImpl;

class HighResolutionTimer final
{
public:
	HighResolutionTimer();
	~HighResolutionTimer();

	// "Tick" the timer to compute the amount of time since the last it was ticked (or since the timer was created).
	void Tick();

	double ElapsedSeconds() const;
	double ElapsedMilliSeconds() const;
	double ElapsedMicroSeconds() const;

private:
	HighResolutionTimerImpl* m_impl;
};