#ifndef TRANSPORT_SYSTEM_H
#define TRANSPORT_SYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <ctime>

using namespace std;

// Template class for generic data storage
template <typename T>
class DataContainer {
private:
    T* data;
    int size;
    int capacity;
public:
    DataContainer();
    ~DataContainer();
    void add(const T& item);
    T* getData() const { return data; }
    int getSize() const { return size; }
    void clear();
    void removeAt(int index);
};

// Forward declarations
class Student;
class Vehicle;
class Route;
class TransportPass;
class Bill;
class Admin;
class Driver;

// Abstract Base Class - User
class User {
protected:
    int userId;
    char* username;
    char* password;
    char* email;

public:
    User();
    User(int id, const char* name, const char* pass, const char* mail);
    virtual ~User();
    User(const User& other);
    User& operator=(const User& other);

    virtual void displayMenu() = 0;
    virtual void displayInfo() = 0;

    int getUserId() const { return userId; }
    const char* getUsername() const { return username; }
    const char* getPassword() const { return password; }
    const char* getEmail() const { return email; }

    bool authenticate(const char* pass) const;

    friend ostream& operator<<(ostream& os, const User& user);
    friend istream& operator>>(istream& is, User& user);
};

// Student class - Derived from User
class Student : public User {
private:
    int rollNo;
    char* department;
    TransportPass* transportPass;
    bool hasActivePass;
public:
    Student();
    Student(int id, const char* name, const char* pass, const char* mail, int roll, const char* dept);
    ~Student();
    Student(const Student& other);
    Student& operator=(const Student& other);
    
    void displayMenu();
    void displayInfo();
    
    void applyForTransport(Route* route, Vehicle* vehicle);
    void viewRegistration();
    void cancelRegistration();
    bool hasPass() const { return hasActivePass; }
    void setTransportPass(TransportPass* pass) { transportPass = pass; hasActivePass = true; }
    TransportPass* getTransportPass() const { return transportPass; }
    int getRollNo() const { return rollNo; }
    const char* getDepartment() const { return department; }
    void setTransportPassDirectly(TransportPass* pass) { transportPass = pass; }
    void setHasActivePass(bool status) { hasActivePass = status; }
    
    friend ostream& operator<<(ostream& os, const Student& student);
    friend istream& operator>>(istream& is, Student& student);
};

// Admin class - Derived from User
class Admin : public User {
public:
    Admin();
    Admin(int id, const char* name, const char* pass, const char* mail);
    ~Admin();
    Admin(const Admin& other);
    Admin& operator=(const Admin& other);
    
    void displayMenu();
    void displayInfo();
};

// Driver class - Derived from User (Optional)
class Driver : public User {
private:
    char* licenseNumber;
    char* phoneNumber;
    int assignedVehicleId;
    double salary;
public:
    Driver();
    Driver(int id, const char* name, const char* pass, const char* mail, const char* license, const char* phone, double sal);
    ~Driver();
    Driver(const Driver& other);
    Driver& operator=(const Driver& other);
    
    void displayMenu();
    void displayInfo();
    
    const char* getLicenseNumber() const { return licenseNumber; }
    const char* getPhoneNumber() const { return phoneNumber; }
    int getAssignedVehicleId() const { return assignedVehicleId; }
    double getSalary() const { return salary; }
    void setAssignedVehicleId(int vehicleId) { assignedVehicleId = vehicleId; }
    void displayDriverInfo();
};

// Abstract Base Class - Vehicle
class Vehicle {
protected:
    int vehicleId;
    char* registrationNumber;
    char* model;
    int capacity;
    bool isAvailable;
    int assignedDriverId;
public:
    Vehicle();
    Vehicle(int id, const char* regNo, const char* mdl, int cap);
    virtual ~Vehicle();
    Vehicle(const Vehicle& other);
    Vehicle& operator=(const Vehicle& other);
    
    virtual void displayDetails() = 0;
    virtual double calculateFuelCost(double distance) = 0;
    virtual double calculateMaintenanceCost() = 0;
    
    int getVehicleId() const { return vehicleId; }
    int getCapacity() const { return capacity; }
    bool getAvailability() const { return isAvailable; }
    void setAvailability(bool avail) { isAvailable = avail; }
    const char* getRegistrationNumber() const { return registrationNumber; }
    int getAssignedDriverId() const { return assignedDriverId; }
    void setAssignedDriverId(int driverId) { assignedDriverId = driverId; }
    
    friend ostream& operator<<(ostream& os, const Vehicle& vehicle);
    friend istream& operator>>(istream& is, Vehicle& vehicle);
};

// Bus class - Derived from Vehicle
class Bus : public Vehicle {
private:
    int busNumber;
    bool hasAC;
    double fuelEfficiency;
public:
    Bus();
    Bus(int id, const char* regNo, const char* mdl, int cap, int busNum, bool ac);
    ~Bus();
    Bus(const Bus& other);
    Bus& operator=(const Bus& other);
    
    void displayDetails();
    double calculateFuelCost(double distance);
    double calculateMaintenanceCost();
};

// Van class - Derived from Vehicle
class Van : public Vehicle {
private:
    int vanNumber;
    int seatingCapacity;
    double fuelEfficiency;
public:
    Van();
    Van(int id, const char* regNo, const char* mdl, int cap, int vanNum, int seatCap);
    ~Van();
    Van(const Van& other);
    Van& operator=(const Van& other);
    
    void displayDetails();
    double calculateFuelCost(double distance);
    double calculateMaintenanceCost();
};

// Route class
class Route {
private:
    int routeId;
    char* startPoint;
    char* endPoint;
    double distance;
    Vehicle* assignedVehicle;
    int currentBookings;
    double farePerKm;
    int totalTrips;
public:
    Route();
    Route(int id, const char* start, const char* end, double dist);
    ~Route();
    Route(const Route& other);
    Route& operator=(const Route& other);
    
    void assignVehicle(Vehicle* vehicle);
    void displayRoute();
    bool isAvailable();
    int getRouteId() const { return routeId; }
    double getDistance() const { return distance; }
    Vehicle* getAssignedVehicle() const { return assignedVehicle; }
    void addBooking() { if(assignedVehicle && currentBookings < assignedVehicle->getCapacity()) currentBookings++; }
    void removeBooking() { if(currentBookings > 0) currentBookings--; }
    int getCurrentBookings() const { return currentBookings; }
    double getFarePerKm() const { return farePerKm; }
    void setFarePerKm(double fare) { farePerKm = fare; }
    void addTrip() { totalTrips++; }
    int getTotalTrips() const { return totalTrips; }
    const char* getStartPoint() const { return startPoint; }
    const char* getEndPoint() const { return endPoint; }
    
    friend ostream& operator<<(ostream& os, const Route& route);
    friend istream& operator>>(istream& is, Route& route);
};

// Bill class
class Bill {
private:
    int billId;
    int studentId;
    double amount;
    double lateFee;
    bool isPaid;
    char* dueDate;
    char* paymentDate;
    int month;
    int year;
public:
    Bill();
    Bill(int id, int studId, double amt, const char* date);
    ~Bill();
    Bill(const Bill& other);
    Bill& operator=(const Bill& other);
    
    void applyLateFee();
    void payBill();
    void displayBill();
    double getTotalAmount() const { return amount + lateFee; }
    int getStudentId() const { return studentId; }
    int getBillId() const { return billId; }
    void setAmount(double amt) { amount = amt; }
    void setLateFee(double fee) { lateFee = fee; }
    void setIsPaid(bool paid) { isPaid = paid; }
    void setPaymentDate(const char* date);
    int getMonth() const { return month; }
    int getYear() const { return year; }
    
    bool operator>(const Bill& other) const;
    Bill operator+(double additional) const;
    
    friend ostream& operator<<(ostream& os, const Bill& bill);
    friend istream& operator>>(istream& is, Bill& bill);
};

// TransportPass class (Composition with Bill)
class TransportPass {
private:
    int passId;
    int studentId;
    int routeId;
    Bill* bill;
    bool isApproved;
    time_t applicationDate;
    time_t approvalDate;
public:
    TransportPass();
    TransportPass(int passId, int studId, int routeId);
    ~TransportPass();
    TransportPass(const TransportPass& other);
    TransportPass& operator=(const TransportPass& other);
    
    void generateBill(double amount, const char* dueDate);
    void approvePass();
    void displayPass();
    bool getApprovalStatus() const { return isApproved; }
    Bill* getBill() const { return bill; }
    int getStudentId() const { return studentId; }
    int getPassId() const { return passId; }
    int getRouteId() const { return routeId; }
    void setApprovalStatus(bool status) { isApproved = status; }
    void setBill(Bill* b) { bill = b; }
    void displayApplicationInfo();
};

// TransportManager class (Aggregation)
class TransportManager {
private:
    DataContainer<Student*> students;
    DataContainer<Admin*> admins;
    DataContainer<Driver*> drivers;
    DataContainer<Vehicle*> vehicles;
    DataContainer<Route*> routes;
    DataContainer<TransportPass*> passes;
    DataContainer<Bill*> bills;
    int nextId;
    
public:
    TransportManager();
    ~TransportManager();
    
    void addStudent(Student* student);
    Student* findStudent(int id);
    void addAdmin(Admin* admin);
    Admin* findAdmin(int id);
    void addDriver(Driver* driver);
    Driver* findDriver(int id);
    void displayAllDrivers();
    bool loginUser(int id, const char* password);
    
    void addVehicle(Vehicle* vehicle);
    void editVehicle(int id);
    void removeVehicle(int id);
    void displayAllVehicles();
    void displayAvailableVehicles();
    Vehicle* findVehicle(int id);
    void assignDriverToVehicle(int driverId, int vehicleId);
    
    void addRoute(Route* route);
    void assignVehicleToRoute(int routeId, int vehicleId);
    void displayAllRoutes();
    Route* findRoute(int id);
    
    void applyForTransport(int studentId, int routeId);
    void viewAllApplications();
    void approveApplication(int passId);
    void rejectApplication(int passId);
    void cancelRegistration(int studentId);
    
    void generateInvoice(int studentId);
    void generateMonthlyInvoice(int studentId, int month, int year);
    void applyLateFines();
    void generateRevenueReport();
    void generateMonthlyRevenueReport(int month, int year);
    void generateRouteUsageReport();
    void generateVehicleReport();
    void generateDriverReport();
    
    void saveData();
    void loadData();
    
    void displayStudentMenu(Student* student);
    void displayAdminMenu(Admin* admin);
    void displayDriverMenu(Driver* driver);
    
    int getNextId() { return nextId++; }
    void setNextId(int id) { nextId = id; }
    void loadNextId();
    void saveNextId();
    
    DataContainer<TransportPass*>& getPasses() { return passes; }
    DataContainer<Bill*>& getBills() { return bills; }
    void cleanupInvalidPasses();
};

#endif