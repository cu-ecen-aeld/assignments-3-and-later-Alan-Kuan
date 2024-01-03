#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* params = (struct thread_data*) thread_param;

    sleep(params->wait_to_obtain_ms / 1000);
    if (pthread_mutex_lock(params->mutex) != 0) {
        return thread_param;
    }

    sleep(params->wait_to_release_ms / 1000);
    if (pthread_mutex_unlock(params->mutex) != 0) {
        return thread_param;
    }

    params->thread_complete_success = true;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data* params = malloc(sizeof(struct thread_data));

    params->mutex = mutex;
    params->wait_to_obtain_ms = wait_to_obtain_ms;
    params->wait_to_release_ms = wait_to_release_ms;
    params->thread_complete_success = false;

    if (pthread_create(thread, NULL, threadfunc, (void*) params) != 0) {
        return false;
    }

    return true;
}

