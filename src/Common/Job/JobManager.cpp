#include "JobManager.hpp"

JobManager* JobManager::instance = nullptr;

JobManager::JobManager(unsigned short number_of_threads)
{
	this->job_threads = vector<JobRunner*>(number_of_threads);
	for (size_t i = 0; i < number_of_threads; i++)
	{
		this->job_threads[i] = new JobRunner();
	}
}

JobManager::~JobManager()
{
	for (size_t i = 0; i < this->job_threads.size(); i++)
	{
		delete this->job_threads[i];
	}
}

void JobManager::create_manager(unsigned short number_of_threads)
{
	JobManager::instance = new JobManager(number_of_threads);
}

JobManager* JobManager::getInstance()
{
	return JobManager::instance;
}

size_t JobManager::getNumberOfThreads()
{
	return JobManager::getInstance()->job_threads.size();
}

void JobManager::runJobs(vector<Job*> jobs)
{
	JobManager* manager = JobManager::getInstance();
	for (size_t i = 0; i < jobs.size(); i++)
	{
		JobManager::getInstance()->job_threads[i]->setJob(jobs[i]);
	}
}

bool JobManager::haveJobsFinished()
{
	JobManager* manager = JobManager::getInstance();
	for (size_t i = 0; i < manager->job_threads.size(); i++)
	{
		if (!manager->job_threads[i]->isJobDone())
		{
			return false;
		}
	}

	return true;
}

void JobManager::waitForJobsToFinish()
{
	while (!JobManager::haveJobsFinished())
	{

	}
}

JobRunner::JobRunner()
{
	this->job_thread = thread(&JobRunner::thread_run, this);
}

JobRunner::~JobRunner()
{
	this->exit = true;
	this->job_thread.join();
}

void JobRunner::setJob(Job* job)
{
	/*this->job_mutex.lock();
	this->job = job;
	this->job_mutex.unlock();*/
	this->atomic_job = job;
}

bool JobRunner::isJobDone()
{
	return this->atomic_job == nullptr;
}

void JobRunner::thread_run()
{
	while (!this->exit)
	{
		Job* job = this->atomic_job;
		if (job != nullptr)
		{
			job->run();
			this->atomic_job = nullptr;
		}
	}
}
