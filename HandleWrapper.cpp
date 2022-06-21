#include "HandleWrapper.h"

HandleWrapper::HandleWrapper() {}

HandleWrapper::HandleWrapper(HANDLE handle) : m_handle(handle) {}

HandleWrapper::~HandleWrapper() 
{
    if (m_handle)
    {
        CloseHandle(m_handle);
    }
}

HandleWrapper::operator HANDLE()
{
    return m_handle;
}

HANDLE HandleWrapper::get() const noexcept
{
    return m_handle;
}
