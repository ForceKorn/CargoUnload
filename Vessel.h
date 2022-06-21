#pragma once

class Vessel
{
public:
    static constexpr size_t SHIP_CAPACITY = 1000;
    static constexpr size_t SHIP_UNLOAD_FACTOR = 5;

public:
    Vessel();

    size_t unloadCargo() noexcept;
    bool   empty() const noexcept;
    size_t getShipID() const noexcept;
    
private:
    static size_t m_id;

    size_t m_shipID = 0;
    size_t m_cargoCapacity = SHIP_CAPACITY;
};

void UnloadTheShip(Vessel& unloadVessel);
