#include "Driver.h"

DriverManager* DriverManager::ActiveDriverManager = 0;

Driver::Driver()
{
}

Driver::~Driver()
{
}

void Driver::activate()
{

}

int Driver::UniquedriverID()
{
    return 0;
}

int Driver::reset()
{
    return 0;
}

void Driver::deactivate()
{

}

DriverManager::DriverManager()
{
    numDrivers = 0;
    DriverManager::ActiveDriverManager = this;
}

DriverManager::~DriverManager()
{

}

void DriverManager::AddDriver(Driver* drv)
{
    drivers[numDrivers] = drv;
    numDrivers++;
}

void DriverManager::activateall()
{
    for (int i = 0; i < numDrivers; i++)
        drivers[i] -> activate();
}

Driver* DriverManager::GetDriver(int UDID)
{
    for (int i = 0; i < numDrivers; i++)
        if (drivers[i] -> UniquedriverID() == UDID)
            return drivers[i];
    return 0;
}