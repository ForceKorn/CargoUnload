#pragma once

#include <windows.h>

class HandleWrapper
{
public:
    HandleWrapper();
    explicit HandleWrapper(HANDLE handle);
    ~HandleWrapper();

    HandleWrapper(const HandleWrapper&) = delete;
    HandleWrapper& operator=(const HandleWrapper) = delete;

    operator HANDLE();
    HANDLE get() const noexcept;

private:
    HANDLE m_handle = 0;
};
