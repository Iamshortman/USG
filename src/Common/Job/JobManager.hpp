#ifndef JOB_MANAGER_HPP
#define JOB_MANAGER_HPP

#include <vector>
using std::vector;

#include <thread>
using std::thread;

#include <mutex>
using std::mutex;

#include <atomic>
using std::atomic;

class Job
{
public:
	virtual void run() 
	{ 
		for (int i = 0; i < 100000; i++)
		{
			
		}
		printf("Thread Done!\n");
	};
};

class JobRunner
{
public:
	JobRunner();
	~JobRunner();

	void setJob(Job* job);
	bool isJobDone();

private:
	void thread_run();
	thread job_thread;

	//mutex job_mutex;
	//Job* job = nullptr;
	atomic<Job*> atomic_job = nullptr;

	bool exit = false;
};

class JobManager
{
public:
	~JobManager();

	static void create_manager(unsigned short number_of_threads);
	static JobManager* getInstance();

	static size_t getNumberOfThreads();

	static void runJobs(vector<Job*> jobs);

	static bool haveJobsFinished();
	static void waitForJobsToFinish();

private:
	//Instance for the Singleton design pattern;
	static JobManager* instance;
	JobManager(unsigned short number_of_threads);

	vector<JobRunner*> job_threads;
};

#include "Common/GLM_Include.hpp"
#include "Common/Transform.hpp"

class JobTest : public Job
{
public:
	JobTest(vector<matrix4>* array_temp, size_t start, size_t end)
	{
		this->array = array_temp;
		this->start = start;
		this->end = end;
	}

	virtual void run()
	{
		for (size_t i = this->start; i < this->end; i++)
		{
			Transform transform;
			(*this->array)[i] = transform.getModelMatrix(vector3D(1.0));
		}
	}

private:
	vector<matrix4>* array;
	size_t start;
	size_t end;
};


#endif //JOB_MANAGER_HPP