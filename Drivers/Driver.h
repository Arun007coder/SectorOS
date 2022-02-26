#ifndef __DRIVER_H
#define __DRIVER_H

class Driver
{
public:
    Driver();
    ~Driver();

    virtual void activate();
    virtual int reset();
    virtual void deactivate();
    virtual int UniquedriverID();
};

class DriverManager
{
public:
    static DriverManager* ActiveDriverManager;
    Driver* drivers[255];
    int numDrivers;
public:
    DriverManager();
    ~DriverManager();
    void AddDriver(Driver*);
    void activateall();
    Driver* GetDriver(int);
};

#endif