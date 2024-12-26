#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cassert>

namespace vld
{
    // A simple RAII-based critical section wrapper
    class criticalsection
    {
    public:
        criticalsection() noexcept
        {
            __try
            {
                InitializeCriticalSection(&m_critRegion);
            }
            __except (GetExceptionCode() == STATUS_NO_MEMORY ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
            {
                assert(false && "Failed to initialize critical section due to insufficient memory.");
            }
        }

        ~criticalsection() noexcept
        {
            DeleteCriticalSection(&m_critRegion);
        }

        criticalsection(const criticalsection&) = delete;
        criticalsection& operator=(const criticalsection&) = delete;
        criticalsection(criticalsection&&) = delete;
        criticalsection& operator=(criticalsection&&) = delete;

        void lock() noexcept
        {
            EnterCriticalSection(&m_critRegion);
        }

        bool try_lock() noexcept
        {
            return TryEnterCriticalSection(&m_critRegion) != 0;
        }

        void unlock() noexcept
        {
            LeaveCriticalSection(&m_critRegion);
        }

        bool is_locked() const noexcept
        {
            return m_critRegion.OwningThread != nullptr;
        }

        bool is_locked_by_current_thread() const noexcept
        {
            // yes, it needs to be compared with the id and not the handle: 
            // https://stackoverflow.com/questions/12675301/why-is-the-owningthread-member-of-critical-section-of-type-handle-when-it-is-de
            return m_critRegion.OwningThread == ULongToHandle(GetCurrentThreadId());
        }

    private:
        CRITICAL_SECTION m_critRegion;
    };

    // RAII-based locker for criticalsection
    class cs_lock
    {
    public:
        explicit cs_lock(criticalsection& critSection) noexcept
            : m_critSection(critSection), m_locked(true)
        {
            m_critSection.lock();
        }

        cs_lock(const cs_lock&) = delete;
        cs_lock& operator=(const cs_lock&) = delete;

        cs_lock(cs_lock&& other) noexcept
            : m_critSection(other.m_critSection), m_locked(other.m_locked)
        {
            other.m_locked = false;
        }

        ~cs_lock() noexcept
        {
            if (m_locked)
            {
                m_critSection.unlock();
            }
        }

        void unlock() noexcept
        {
            if (m_locked)
            {
                m_critSection.unlock();
                m_locked = false;
            }
        }

    private:
        criticalsection& m_critSection;
        bool m_locked;
    };
}
