#pragma once

#include <iostream>
#include <array>

#include <windows.h>

template<typename>
struct array_size;

template<typename T, std::size_t N>
struct array_size<std::array<T, N>>
{
    static const std::size_t size = N;
};


template<typename HandleArray>
class HandlesArrayWrapper
{
private:
    using container_t = HandleArray;

public:
    HandlesArrayWrapper(container_t& handles) : m_handles(handles) {}

    ~HandlesArrayWrapper()
    {
        for (const auto& handle : m_handles)
        {
            if (handle)
            {
                CloseHandle(handle);
            }
        }
    }

private:
    container_t& m_handles;
};

class MutexGuard
{
public:
    MutexGuard(HANDLE mutex) : m_mutex(mutex)
    {
        WaitForSingleObject(m_mutex, INFINITE);
    }

    ~MutexGuard()
    {
        ReleaseMutex(m_mutex);
    }

private:
    HANDLE m_mutex;
};

class SemaphoreGuard
{
public:
    SemaphoreGuard(HANDLE semaphore) : m_semaphore(semaphore)
    {
        WaitForSingleObject(m_semaphore, 0);
    }

    ~SemaphoreGuard()
    {
        ReleaseSemaphore(m_semaphore, 1, 0);
    }

private:
    HANDLE m_semaphore;
};
