#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <array>

#include <windows.h>

#include "utils.h"
#include "Vessel.h"
#include "HandleWrapper.h"
#include "CompileMacroFlags.h"


constexpr size_t MAX_THREADS = 3;
constexpr size_t SHIPS       = 50;

HandleWrapper gs_semaphoreWrap { CreateSemaphore(0, MAX_THREADS, MAX_THREADS, 0) };

#if COUT_SYNC_PRINT
    HandleWrapper gs_mutexWrap { CreateMutex(0, FALSE, 0) };
#endif


DWORD WINAPI threadWorker(LPVOID value)
{
    SemaphoreGuard guardThreadWorker(gs_semaphoreWrap.get());

    Vessel* unloadVessel = reinterpret_cast<Vessel*>(value);
    if (unloadVessel)
    {
#if COUT_SYNC_PRINT
        MutexGuard guardUnloadTheShip(gs_mutexWrap.get());
#endif
        UnloadTheShip(*unloadVessel);
    }

    return 0;
}

#if COUT_SYNC_PRINT
template<typename HandleArray, typename VesselArray>
void initWorkersWithVessels(HandleArray& workers, VesselArray& unloadGroup, HANDLE mutexHandle)
{
    static_assert(array_size<HandleArray>::size == array_size<VesselArray>::size,
        "Thread array size and Vessel array size are different. Should be equal!");

    MutexGuard guardCreateThread(mutexHandle);
    for (size_t i = 0; i < workers.size(); ++i)
    {
        workers[i] = CreateThread(0, 0, threadWorker, reinterpret_cast<void*>(&unloadGroup[i]), 0, 0);
        if (!workers[i])
        {
            std::cout << "CreateThread <" << i << "> error: " << GetLastError() << '\n';
            system("pause");
        }
    }
    std::cout << '\n';
}
#endif

template<typename HandleArray, typename VesselArray>
void initWorkersWithVessels(HandleArray& workers, VesselArray& unloadGroup)
{
    static_assert(array_size<HandleArray>::size == array_size<VesselArray>::size,
        "Thread array size and Vessel array size are different. Should be equal!");

    for (size_t i = 0; i < workers.size(); ++i)
    {
        workers[i] = CreateThread(0, 0, threadWorker, reinterpret_cast<void*>(&unloadGroup[i]), 0, 0);
        if (!workers[i])
        {
            std::cout << "CreateThread <" << i << "> error: " << GetLastError() << '\n';
            system("pause");
        }
    }
    std::cout << '\n';
}

template<typename Vessels>
void printUnloadVesselsEmptyStatus(const Vessels& unloadGroup, const char* message = "")
{
    std::cout << message << "Are workers empty : ";
    std::cout << std::boolalpha;
    std::cout << std::all_of(unloadGroup.begin(), unloadGroup.end(), 
                    std::mem_fn(&Vessel::empty)) << '\n';
}


int main()
{
    if (!gs_semaphoreWrap.get())
    {
        std::cout << "CreateSemaphore error! Terminating\n";
        return 0;
    }

#if COUT_SYNC_PRINT
    if (!gs_mutexWrap.get())
    {
        std::cout << "CreateMutex error! Terminating\n";
        return 0;
    }
#endif

    std::array<Vessel, SHIPS> unloadGroup  = {};
    std::array<HANDLE, SHIPS> workers      = {};
    HandlesArrayWrapper       workersGuard = { workers };

    printUnloadVesselsEmptyStatus(unloadGroup, "Before tasks. ");

#if COUT_SYNC_PRINT
    initWorkersWithVessels(workers, unloadGroup, gs_mutexWrap.get());
#else
    initWorkersWithVessels(workers, unloadGroup);
#endif
    WaitForMultipleObjects(SHIPS, &workers[0], TRUE, INFINITE);

    printUnloadVesselsEmptyStatus(unloadGroup, "After tasks. ");
    
    system("pause");
    return 0;
}
