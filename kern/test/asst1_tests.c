#include <types.h>
#include <lib.h>
#include <test.h>
#include <synch.h>
#include <thread.h>

#define THREAD_COUNT 10

static struct lock *shared_testing_lock;


static int correct_condition_variable()
{
	kprintf("correct_condition_variable successful\n");
	return 0;
}

static int correct_thread_join()
{
	kprintf("correct_thread_join successful\n");
	return 0;
}


static void lock_holding_tester(void *unnecessary, unsigned long thread_num)
{
	(void)  unnecessary;
	(void) thread_num;

	if(lock_do_i_hold(shared_testing_lock) == 0)
	{
		lock_acquire(shared_testing_lock);

		if(lock_do_i_hold(shared_testing_lock) == 0)
		{
			panic("Failure in asst1_tests: correct_lock_holding\n");
		}
		else
		{
			thread_yield();

			if(lock_do_i_hold(shared_testing_lock) == 0)
			{
				panic("Failure in asst1_tests: correct_lock_holding\n");
			}
			else
			{
				lock_release(shared_testing_lock);

				if(lock_do_i_hold(shared_testing_lock) == 0)
				{
					return;
				}
				else
				{
					panic("Failure in asst1_tests: correct_lock_holding\n");	
				}
			}
		}
	}
	else
	{
		panic("Failure in asst1_tests: correct_lock_holding\n");	
	}
}

static int correct_lock_holding()
{

	struct semaphore *test_sem = sem_create("test_sem", 0);

	for(int thread_num = 0; thread_num < THREAD_COUNT; thread_num++)
	{
		thread_fork("correct_lock_holding", NULL, lock_holding_tester, NULL, thread_num);
		V(test_sem);
	}

	for(int thread_num = 0; thread_num < THREAD_COUNT; thread_num++)
	{
		P(test_sem);
	}



	kprintf("correct_lock_holding successful\n");
	return 0;
}

static void initializtion()
{
	shared_testing_lock = lock_create("shared_testing_lock");

	if(shared_testing_lock == NULL)
	{
		panic("Failure in asst1_tests: initializtion\n");
	}
}

int asst1_tests(int nargs, char **args)
{
	kprintf("START ASST1_TESTS!\n");

	initializtion(nargs, args);
	KASSERT(correct_lock_holding(nargs, args) == 0);
	KASSERT(correct_thread_join(nargs, args) == 0);
	KASSERT(correct_condition_variable(nargs, args) == 0);

	kprintf("ASST1_TESTS COMPLETED!\n");

	return 0;
}