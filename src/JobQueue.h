#pragma once

#include "pch.h"

typedef std::function<void()> job_t;

class JobQueue
{
public:
	void Add(job_t Job);
	void Run();

private:
	std::queue<job_t> m_Queue;
};

inline JobQueue g_JobQueue;

#define QUEUE_JOB(...) g_JobQueue.Add([__VA_ARGS__]() {
#define END_JOB() });
