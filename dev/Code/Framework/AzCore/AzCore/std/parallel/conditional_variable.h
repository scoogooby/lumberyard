/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once

#include <AzCore/std/parallel/mutex.h>
#include <AzCore/std/parallel/lock.h>
#include <AzCore/std/chrono/chrono.h>

namespace AZStd
{
    /**
     * Condition variables provide synchronization primitives used to block a thread until notified by some other
     * thread that some condition is met or until a system time is reached. \ref C++0x (30.5)
     */
    class condition_variable
    {
    public:
        typedef native_cond_var_handle_type native_handle_type;

        condition_variable();
        ~condition_variable();

        void notify_one();
        void notify_all();
        void wait(unique_lock<mutex>& lock);
        template <class Predicate>
        void wait(unique_lock<mutex>& lock, Predicate pred);
        template <class Clock, class Duration>
        bool wait_until(unique_lock<mutex>& lock, const chrono::time_point<Clock, Duration>& abs_time);
        template <class Clock, class Duration, class Predicate>
        bool wait_until(unique_lock<mutex>& lock, const chrono::time_point<Clock, Duration>& abs_time, Predicate pred);
        template <class Rep, class Period>
        bool wait_for(unique_lock<mutex>& lock, const chrono::duration<Rep, Period>& rel_time);
        template <class Rep, class Period, class Predicate>
        bool wait_for(unique_lock<mutex>& lock, const chrono::duration<Rep, Period>& rel_time, Predicate pred);
        native_handle_type native_handle();

        // Extensions
        condition_variable(const char* name);
    private:
        condition_variable(const condition_variable&);
        condition_variable& operator=(const condition_variable&);

        native_cond_var_data_type m_cond_var;
    };

    /**
     * A Lock type shall meet the requirements for a Mutex type, except that try_lock is not required.
     * \note  All of the standard mutex types meet this requirement.
     */
    class condition_variable_any
    {
    public:
        typedef native_cond_var_handle_type native_handle_type;

        condition_variable_any();
        ~condition_variable_any();

        void notify_one();
        void notify_all();
        template <class Lock>
        void wait(Lock& lock);
        template <class Lock, class Predicate>
        void wait(Lock& lock, Predicate pred);
        template <class Lock, class Clock, class Duration>
        bool wait_until(Lock& lock, const chrono::time_point<Clock, Duration>& abs_time);
        template <class Lock, class Clock, class Duration, class Predicate>
        bool wait_until(Lock& lock, const chrono::time_point<Clock, Duration>& abs_time, Predicate pred);
        template <class Lock, class Rep, class Period>
        bool wait_for(Lock& lock, const chrono::duration<Rep, Period>& rel_time);
        template <class Lock, class Rep, class Period, class Predicate>
        bool wait_for(Lock& lock, const chrono::duration<Rep, Period>& rel_time, Predicate pred);
        native_handle_type native_handle();

        // Extensions
        condition_variable_any(const char* name);
    private:
        condition_variable_any(const condition_variable_any&);
        condition_variable_any& operator=(const condition_variable_any&);

        native_cond_var_data_type m_cond_var;
        native_mutex_data_type m_mutex;
    };
}

#if AZ_TRAIT_USE_WINDOWS_CONDITIONAL_VARIABLE
    #include <AzCore/std/parallel/internal/conditional_variable_win.h>
#define AZ_RESTRICTED_SECTION_IMPLEMENTED
#elif defined(AZ_RESTRICTED_PLATFORM)
#include AZ_RESTRICTED_FILE(conditional_variable_h, AZ_RESTRICTED_PLATFORM)
#endif
#if defined(AZ_RESTRICTED_SECTION_IMPLEMENTED)
#undef AZ_RESTRICTED_SECTION_IMPLEMENTED
#elif defined (AZ_PLATFORM_LINUX) || defined(AZ_PLATFORM_ANDROID) || defined(AZ_PLATFORM_APPLE)
    #include <AzCore/std/parallel/internal/conditional_variable_linux.h>
#else
    #error Platform not supported
#endif
