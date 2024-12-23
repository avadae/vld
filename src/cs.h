#pragma once

#include <windows.h>
#include <cassert>

// Modernized criticalsection class with RAII for resource management
namespace vld
{
    class criticalsection
    {
    public:
        // Constructor: Initializes the critical section
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

        // Destructor: Deletes the critical section
        ~criticalsection() noexcept
        {
            DeleteCriticalSection(&m_critRegion);
        }

        // Deleted copy constructor and copy assignment operator
        criticalsection(const criticalsection&) = delete;
        criticalsection& operator=(const criticalsection&) = delete;

        // Deleted move constructor and move assignment operator
        criticalsection(criticalsection&&) = delete;
        criticalsection& operator=(criticalsection&&) = delete;

        // Enter the critical section
        void lock() noexcept
        {
            EnterCriticalSection(&m_critRegion);
        }

        // Try to enter the critical section (non-blocking)
        bool try_lock() noexcept
        {
            return TryEnterCriticalSection(&m_critRegion) != 0;
        }

        // Leave the critical section
        void unlock() noexcept
        {
            LeaveCriticalSection(&m_critRegion);
        }

        // Check if the critical section is locked (non-standard utility)
        bool is_locked() const noexcept
        {
            return m_critRegion.OwningThread != nullptr;
        }

        // Check if the current thread owns the lock (non-standard utility)
        bool is_locked_by_current_thread() const noexcept
        {
            // yes, it needs to be compared with the id and not the handle: 
            // https://stackoverflow.com/questions/12675301/why-is-the-owningthread-member-of-critical-section-of-type-handle-when-it-is-de
            return m_critRegion.OwningThread == (HANDLE)GetCurrentThreadId();
        }

    private:
        CRITICAL_SECTION m_critRegion;
    };

    // RAII-based locker for CriticalSection
    class scoped_lock
    {
    public:
        explicit scoped_lock(criticalsection& critSection) noexcept
            : m_critSection(critSection), m_locked(true)
        {
            m_critSection.lock();
        }

        // Deleted copy constructor and copy assignment operator
        scoped_lock(const scoped_lock&) = delete;
        scoped_lock& operator=(const scoped_lock&) = delete;

        // Allow move semantics
        scoped_lock(scoped_lock&& other) noexcept
            : m_critSection(other.m_critSection), m_locked(other.m_locked)
        {
            other.m_locked = false;
        }

        // Destructor: Automatically releases the lock if still held
        ~scoped_lock() noexcept
        {
            if (m_locked)
            {
                m_critSection.unlock();
            }
        }

        // Manually unlock the critical section
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
