#include "Vessel.h"
#include "CompileMacroFlags.h"

#include <iostream>
#include <algorithm>
#include <ctime>

size_t Vessel::m_id = 0;

Vessel::Vessel() : m_shipID(++m_id) { std::srand(static_cast<size_t>(time(0))); }

size_t Vessel::unloadCargo() noexcept
{
    const size_t k_unloadToFitShipCapacity 
        = std::min(static_cast<size_t>((std::rand() % SHIP_UNLOAD_FACTOR) + 1), m_cargoCapacity);

    m_cargoCapacity -= k_unloadToFitShipCapacity;
    return k_unloadToFitShipCapacity;
}

bool Vessel::empty() const noexcept
{
    return !m_cargoCapacity;
}

size_t Vessel::getShipID() const noexcept
{
    return m_shipID;
}

void UnloadTheShip(Vessel& unloadVessel)
{
    while (!unloadVessel.empty())
    {
#if COUT_SYNC_PRINT
        std::cout << "Vessel#" << unloadVessel.getShipID()
            << " -> unloaded cargo: " << unloadVessel.unloadCargo() << '\n';
#else
        unloadVessel.unloadCargo();
#endif
    }
}