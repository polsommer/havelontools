////////////////////////////////////////
//  Event.cpp
//
//  Purpose:
// 	    1. Implementation of the CEvent class.
//
//  Revisions:
// 	    07/10/2001  Created                                    
//

#include "./Event.h"

namespace Base
{

    CEvent::CEvent()
    {
        mInitialized = false;
        if (pthread_cond_init( &mCond, NULL ) != 0)
        {
            return;
        }

        if (pthread_mutex_init( &mMutex, NULL ) != 0)
        {
            pthread_cond_destroy( &mCond );
            return;
        }
        mInitialized = true;
    }

    CEvent::~CEvent()
    {
        if (mInitialized)
        {
            pthread_cond_destroy(&mCond);
            pthread_mutex_destroy(&mMutex);
        }
    }

    int32 CEvent::Wait(uint32 timeout)
    {
        if (!mInitialized)
            return CEvent::eWAIT_ERROR;

        int result;
        if (!timeout)
            result = pthread_cond_wait(&mCond, &mMutex);
        else
        {
            struct timespec wake_time;

            // linux 2.0 is not fully POSIX compliant with high-resolution timers
            // so we use the older gettimeofday()
            struct timeval tv;

            // get high resolution time and convert to POSIX 4 struct
            if (gettimeofday(&tv, NULL) != 0)
                return CEvent::eWAIT_ERROR;
            TIMEVAL_TO_TIMESPEC(&tv, &wake_time);

            // add time out
            wake_time.tv_sec  += timeout/1000;
            wake_time.tv_nsec += (timeout%1000)*1000000;

            // normalize new time
            wake_time.tv_sec  += wake_time.tv_nsec / 1000000000;
            wake_time.tv_nsec %= 1000000000;

            // wait on condition with time out
            result = pthread_cond_timedwait(&mCond, &mMutex, &wake_time );
        }

        if (result == 0 || result == EINTR)
            return CEvent::eWAIT_SIGNAL;
        else if (result == ETIMEDOUT)
            return CEvent::eWAIT_TIMEOUT;
        else
            return CEvent::eWAIT_ERROR;
    }

}
