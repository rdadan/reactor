#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "../util/NonCopy.h"

#include <pthread.h>

namespace THREADPOOL
{
    class Mutex : public NonCopy
    {
    public:
        Mutex();
        ~Mutex();
        pthread_mutex_t *getMutexPtr();
        void mutexLock();
        void mutexUnlock();

    private:
        pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
        bool _isLocking;
    };

    class AutoMutex
    {
    public:
        AutoMutex(Mutex &mutex)
            : _autoMutex(mutex)
        {
            _autoMutex.mutexLock();
        }

        ~AutoMutex()
        {
            _autoMutex.mutexUnlock();
        }

    private:
        Mutex &_autoMutex;
    };

} // end space

#endif