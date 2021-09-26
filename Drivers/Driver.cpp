#include "Driver.h"

Driver::Driver()
{

}

Driver::~Driver()
{
}

void Driver::activate()
{

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