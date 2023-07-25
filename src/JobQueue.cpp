#include "pch.h"
#include "JobQueue.h"

void JobQueue::Add(job_t Job)
{
	m_Queue.push(Job);
}

void JobQueue::Run()
{
	while (!m_Queue.empty())
	{
		m_Queue.front()();
		m_Queue.pop();
	}
}
