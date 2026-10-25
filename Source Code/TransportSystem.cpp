#include "TransportSystem.h"
#include <windows.h>
#include <sstream>
using namespace std;

// ================= COLOR FUNCTION =================

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Template class implementation
template <typename T>
DataContainer<T>::DataContainer() : data(nullptr), size(0), capacity(10) {
    data = new T[capacity];
}

template <typename T>
DataContainer<T>::~DataContainer() {
    delete[] data;
}

template <typename T>
void DataContainer<T>::add(const T& item) {
    if (size >= capacity) {
        capacity *= 2;
        T* temp = new T[capacity];
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
    }
    data[size++] = item;
}

template <typename T>
void DataContainer<T>::removeAt(int index) {
    if (index < 0 || index >= size) return;
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
}

template <typename T>
void DataContainer<T>::clear() {
    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 10;
    data = new T[capacity];
}

// Explicit template instantiation
template class DataContainer<Student*>;
template class DataContainer<Admin*>;
template class DataContainer<Driver*>;
template class DataContainer<Vehicle*>;
template class DataContainer<Route*>;
template class DataContainer<TransportPass*>;
template class DataContainer<Bill*>;

// User class implementation (same as before)
User::User() : userId(0), username(nullptr), password(nullptr), email(nullptr) {}

User::User(int id, const char* name, const char* pass, const char* mail) : userId(id) {
    username = new char[strlen(name) + 1];
    strcpy(username, name);
    password = new char[strlen(pass) + 1];
    strcpy(password, pass);
    email = new char[strlen(mail) + 1];
    strcpy(email, mail);
}

User::~User() {
    delete[] username;
    delete[] password;
    delete[] email;
}

User::User(const User& other) : userId(other.userId) {
    username = new char[strlen(other.username) + 1];
    strcpy(username, other.username);
    password = new char[strlen(other.password) + 1];
    strcpy(password, other.password);
    email = new char[strlen(other.email) + 1];
    strcpy(email, other.email);
}

User& User::operator=(const User& other) {
    if (this != &other) {
        userId = other.userId;
        delete[] username;
        delete[] password;
        delete[] email;
        username = new char[strlen(other.username) + 1];
        strcpy(username, other.username);
        password = new char[strlen(other.password) + 1];
        strcpy(password, other.password);
        email = new char[strlen(other.email) + 1];
        strcpy(email, other.email);
    }
    return *this;
}

bool User::authenticate(const char* pass) const {
    return strcmp(password, pass) == 0;
}

ostream& operator<<(ostream& os, const User& user) {
    os << user.userId << "\n" << user.username << "\n" << user.password << "\n" << user.email;
    return os;
}

// Student class implementation (same as before)
Student::Student() : User(), rollNo(0), department(nullptr), transportPass(nullptr), hasActivePass(false) {}

Student::Student(int id, const char* name, const char* pass, const char* mail, int roll, const char* dept)
    : User(id, name, pass, mail), rollNo(roll), transportPass(nullptr), hasActivePass(false) {
    department = new char[strlen(dept) + 1];
    strcpy(department, dept);
}

Student::~Student() {
    delete[] department;
    delete transportPass;
}

Student::Student(const Student& other) : User(other), rollNo(other.rollNo), transportPass(nullptr), hasActivePass(other.hasActivePass) {
    department = new char[strlen(other.department) + 1];
    strcpy(department, other.department);
    if (other.transportPass) {
        transportPass = new TransportPass(*other.transportPass);
    }
}

Student& Student::operator=(const Student& other) {
    if (this != &other) {
        User::operator=(other);
        rollNo = other.rollNo;
        hasActivePass = other.hasActivePass;
        delete[] department;
        department = new char[strlen(other.department) + 1];
        strcpy(department, other.department);
        delete transportPass;
        transportPass = other.transportPass ? new TransportPass(*other.transportPass) : nullptr;
    }
    return *this;
}

void Student::displayMenu() {
    setColor(10);
    cout << "\n========== STUDENT MENU ==========\n";
    setColor(15);
    cout << "1. View Routes\n";
    cout << "2. Apply for Transport\n";
    cout << "3. View My Registration\n";
    cout << "4. Cancel Registration\n";
    cout << "5. Pay Transport Fee\n";
    cout << "6. View Invoice\n";
    cout << "7. Logout\n";
    setColor(11);
    cout << "Enter choice: ";
    setColor(15);
}

void Student::displayInfo() {
    cout << "\n--- Student Information ---\n";
    cout << "ID: " << userId << "\nName: " << username << "\nRoll No: " << rollNo 
         << "\nDepartment: " << department << "\nEmail: " << email << "\n";
}

void Student::applyForTransport(Route* route, Vehicle* vehicle) {
    if (vehicle && vehicle->getAvailability() && route && !hasActivePass) {
        transportPass = new TransportPass(1000 + userId, userId, route->getRouteId());
        double fee = route->getDistance() * 2.5 * 30; // Monthly fee based on distance
        transportPass->generateBill(fee, "2026-06-10");
        hasActivePass = true;
        route->addBooking();
        setColor(10);
        cout << "Application submitted successfully! Waiting for approval.\n";
        setColor(15);
    } else {
        setColor(12);
        cout << "Cannot apply. Either already registered or no availability!\n";
        setColor(15);
    }
}

void Student::viewRegistration() {
    if (hasActivePass && transportPass) {
        setColor(11);
        cout << "\n===== MY REGISTRATION =====\n";
        setColor(15);
        transportPass->displayPass();
        if (transportPass->getBill()) {
            transportPass->getBill()->displayBill();
        }
    } else {
        setColor(12);
        cout << "No active registration found.\n";
        setColor(15);
    }
}

void Student::cancelRegistration() {
    if (hasActivePass && transportPass) {
        delete transportPass;
        transportPass = nullptr;
        hasActivePass = false;
        setColor(10);
        cout << "Registration cancelled successfully.\n";
        setColor(15);
    } else {
        setColor(12);
        cout << "No active registration to cancel.\n";
        setColor(15);
    }
}

// Admin class implementation
Admin::Admin() : User() {}

Admin::Admin(int id, const char* name, const char* pass, const char* mail) : User(id, name, pass, mail) {}

Admin::~Admin() {}

Admin::Admin(const Admin& other) : User(other) {}

Admin& Admin::operator=(const Admin& other) {
    if (this != &other) {
        User::operator=(other);
    }
    return *this;
}

void Admin::displayMenu() {
    setColor(14);
    cout << "\n========== ADMIN MENU ==========\n";
    setColor(15);
    cout << "1. Add Vehicle\n";
    cout << "2. Edit Vehicle\n";
    cout << "3. Remove Vehicle\n";
    cout << "4. View Available Vehicles\n";
    cout << "5. Add Route\n";
    cout << "6. Assign Vehicle to Route\n";
    cout << "7. Add Driver\n";
    cout << "8. Assign Driver to Vehicle\n";
    cout << "9. View Applications\n";
    cout << "10. Approve/Reject Requests\n";
    cout << "11. Generate Reports\n";
    cout << "12. Apply Late Fines\n";
    cout << "13. Save Data\n";
    cout << "14. Logout\n";
    setColor(11);
    cout << "Enter choice: ";
    setColor(15);
}

void Admin::displayInfo() {
    cout << "\n--- Admin Information ---\n";
    cout << "ID: " << userId << "\nName: " << username << "\nEmail: " << email << "\n";
}

// Driver class implementation
Driver::Driver() : User(), licenseNumber(nullptr), phoneNumber(nullptr), assignedVehicleId(-1), salary(0) {}

Driver::Driver(int id, const char* name, const char* pass, const char* mail, const char* license, const char* phone, double sal)
    : User(id, name, pass, mail), assignedVehicleId(-1), salary(sal) {
    licenseNumber = new char[strlen(license) + 1];
    strcpy(licenseNumber, license);
    phoneNumber = new char[strlen(phone) + 1];
    strcpy(phoneNumber, phone);
}

Driver::~Driver() {
    delete[] licenseNumber;
    delete[] phoneNumber;
}

Driver::Driver(const Driver& other) : User(other), assignedVehicleId(other.assignedVehicleId), salary(other.salary) {
    licenseNumber = new char[strlen(other.licenseNumber) + 1];
    strcpy(licenseNumber, other.licenseNumber);
    phoneNumber = new char[strlen(other.phoneNumber) + 1];
    strcpy(phoneNumber, other.phoneNumber);
}

Driver& Driver::operator=(const Driver& other) {
    if (this != &other) {
        User::operator=(other);
        assignedVehicleId = other.assignedVehicleId;
        salary = other.salary;
        delete[] licenseNumber;
        delete[] phoneNumber;
        licenseNumber = new char[strlen(other.licenseNumber) + 1];
        strcpy(licenseNumber, other.licenseNumber);
        phoneNumber = new char[strlen(other.phoneNumber) + 1];
        strcpy(phoneNumber, other.phoneNumber);
    }
    return *this;
}

void Driver::displayMenu() {
    setColor(13);
    cout << "\n========== DRIVER MENU ==========\n";
    setColor(15);
    cout << "1. View My Assigned Vehicle\n";
    cout << "2. Update Availability\n";
    cout << "3. View My Schedule\n";
    cout << "4. Logout\n";
    setColor(11);
    cout << "Enter choice: ";
    setColor(15);
}

void Driver::displayInfo() {
    cout << "\n--- Driver Information ---\n";
    cout << "ID: " << userId << "\nName: " << username << "\nLicense: " << licenseNumber 
         << "\nPhone: " << phoneNumber << "\nSalary: $" << salary << "\n";
}

void Driver::displayDriverInfo() {
    displayInfo();
    if (assignedVehicleId != -1) {
        cout << "Assigned Vehicle ID: " << assignedVehicleId << "\n";
    } else {
        cout << "No vehicle assigned\n";
    }
}

// Vehicle class implementation
Vehicle::Vehicle() : vehicleId(0), registrationNumber(nullptr), model(nullptr), capacity(0), isAvailable(true), assignedDriverId(-1) {}

Vehicle::Vehicle(int id, const char* regNo, const char* mdl, int cap) 
    : vehicleId(id), capacity(cap), isAvailable(true), assignedDriverId(-1) {
    registrationNumber = new char[strlen(regNo) + 1];
    strcpy(registrationNumber, regNo);
    model = new char[strlen(mdl) + 1];
    strcpy(model, mdl);
}

Vehicle::~Vehicle() {
    delete[] registrationNumber;
    delete[] model;
}

Vehicle::Vehicle(const Vehicle& other) : vehicleId(other.vehicleId), capacity(other.capacity), isAvailable(other.isAvailable), assignedDriverId(other.assignedDriverId) {
    registrationNumber = new char[strlen(other.registrationNumber) + 1];
    strcpy(registrationNumber, other.registrationNumber);
    model = new char[strlen(other.model) + 1];
    strcpy(model, other.model);
}

Vehicle& Vehicle::operator=(const Vehicle& other) {
    if (this != &other) {
        vehicleId = other.vehicleId;
        capacity = other.capacity;
        isAvailable = other.isAvailable;
        assignedDriverId = other.assignedDriverId;
        delete[] registrationNumber;
        delete[] model;
        registrationNumber = new char[strlen(other.registrationNumber) + 1];
        strcpy(registrationNumber, other.registrationNumber);
        model = new char[strlen(other.model) + 1];
        strcpy(model, other.model);
    }
    return *this;
}

// Bus implementation
Bus::Bus() : Vehicle(), busNumber(0), hasAC(false), fuelEfficiency(3.5) {}

Bus::Bus(int id, const char* regNo, const char* mdl, int cap, int busNum, bool ac) 
    : Vehicle(id, regNo, mdl, cap), busNumber(busNum), hasAC(ac), fuelEfficiency(3.5) {}

Bus::~Bus() {}

Bus::Bus(const Bus& other) : Vehicle(other), busNumber(other.busNumber), hasAC(other.hasAC), fuelEfficiency(other.fuelEfficiency) {}

Bus& Bus::operator=(const Bus& other) {
    if (this != &other) {
        Vehicle::operator=(other);
        busNumber = other.busNumber;
        hasAC = other.hasAC;
        fuelEfficiency = other.fuelEfficiency;
    }
    return *this;
}

void Bus::displayDetails() {
    cout << "BUS - ID:" << vehicleId << ", Reg:" << registrationNumber << ", Model:" << model 
         << ", Capacity:" << capacity << ", Bus#:" << busNumber << ", AC:" << (hasAC ? "Yes" : "No") 
         << ", Available:" << (isAvailable ? "Yes" : "No");
    if (assignedDriverId != -1) cout << ", Driver ID:" << assignedDriverId;
    cout << "\n";
}

double Bus::calculateFuelCost(double distance) {
    return distance * fuelEfficiency * (hasAC ? 1.2 : 1.0);
}

double Bus::calculateMaintenanceCost() {
    return capacity * 50.0 + (hasAC ? 200.0 : 100.0);
}

// Van implementation
Van::Van() : Vehicle(), vanNumber(0), seatingCapacity(0), fuelEfficiency(2.5) {}

Van::Van(int id, const char* regNo, const char* mdl, int cap, int vanNum, int seatCap) 
    : Vehicle(id, regNo, mdl, cap), vanNumber(vanNum), seatingCapacity(seatCap), fuelEfficiency(2.5) {}

Van::~Van() {}

Van::Van(const Van& other) : Vehicle(other), vanNumber(other.vanNumber), seatingCapacity(other.seatingCapacity), fuelEfficiency(other.fuelEfficiency) {}

Van& Van::operator=(const Van& other) {
    if (this != &other) {
        Vehicle::operator=(other);
        vanNumber = other.vanNumber;
        seatingCapacity = other.seatingCapacity;
        fuelEfficiency = other.fuelEfficiency;
    }
    return *this;
}

void Van::displayDetails() {
    cout << "VAN - ID:" << vehicleId << ", Reg:" << registrationNumber << ", Model:" << model 
         << ", Capacity:" << capacity << ", Van#:" << vanNumber << ", Seats:" << seatingCapacity 
         << ", Available:" << (isAvailable ? "Yes" : "No");
    if (assignedDriverId != -1) cout << ", Driver ID:" << assignedDriverId;
    cout << "\n";
}

double Van::calculateFuelCost(double distance) {
    return distance * fuelEfficiency;
}

double Van::calculateMaintenanceCost() {
    return seatingCapacity * 30.0;
}

// Route implementation
Route::Route() : routeId(0), startPoint(nullptr), endPoint(nullptr), distance(0), assignedVehicle(nullptr), currentBookings(0), farePerKm(2.5), totalTrips(0) {}

Route::Route(int id, const char* start, const char* end, double dist) 
    : routeId(id), distance(dist), assignedVehicle(nullptr), currentBookings(0), farePerKm(2.5), totalTrips(0) {
    startPoint = new char[strlen(start) + 1];
    strcpy(startPoint, start);
    endPoint = new char[strlen(end) + 1];
    strcpy(endPoint, end);
}

Route::~Route() {
    delete[] startPoint;
    delete[] endPoint;
}

Route::Route(const Route& other) : routeId(other.routeId), distance(other.distance), assignedVehicle(other.assignedVehicle), currentBookings(other.currentBookings), farePerKm(other.farePerKm), totalTrips(other.totalTrips) {
    startPoint = new char[strlen(other.startPoint) + 1];
    strcpy(startPoint, other.startPoint);
    endPoint = new char[strlen(other.endPoint) + 1];
    strcpy(endPoint, other.endPoint);
}

Route& Route::operator=(const Route& other) {
    if (this != &other) {
        routeId = other.routeId;
        distance = other.distance;
        assignedVehicle = other.assignedVehicle;
        currentBookings = other.currentBookings;
        farePerKm = other.farePerKm;
        totalTrips = other.totalTrips;
        delete[] startPoint;
        delete[] endPoint;
        startPoint = new char[strlen(other.startPoint) + 1];
        strcpy(startPoint, other.startPoint);
        endPoint = new char[strlen(other.endPoint) + 1];
        strcpy(endPoint, other.endPoint);
    }
    return *this;
}

void Route::assignVehicle(Vehicle* vehicle) {
    assignedVehicle = vehicle;
   // cout << "Vehicle assigned to route successfully.\n";
}

void Route::displayRoute() {
    cout << "Route ID:" << routeId << ", " << startPoint << " -> " << endPoint 
         << ", Distance:" << distance << "km, Fare: Rs" << farePerKm << "/km"
         << ", Bookings:" << currentBookings << "/";
    if (assignedVehicle) cout << assignedVehicle->getCapacity() << "\n";
    else cout << "No vehicle assigned\n";
}

bool Route::isAvailable() {
    return assignedVehicle && assignedVehicle->getAvailability() && currentBookings < assignedVehicle->getCapacity();
}

// Bill implementation
Bill::Bill() : billId(0), studentId(0), amount(0), lateFee(0), isPaid(false), dueDate(nullptr), paymentDate(nullptr), month(0), year(0) {}

Bill::Bill(int id, int studId, double amt, const char* date) : billId(id), studentId(studId), amount(amt), lateFee(0), isPaid(false), paymentDate(nullptr) {
    dueDate = new char[strlen(date) + 1];
    strcpy(dueDate, date);
    
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    month = timeinfo->tm_mon + 1;
    year = timeinfo->tm_year + 1900;
}

Bill::~Bill() { 
    delete[] dueDate;
    delete[] paymentDate;
}

Bill::Bill(const Bill& other) : billId(other.billId), studentId(other.studentId), amount(other.amount), lateFee(other.lateFee), isPaid(other.isPaid), month(other.month), year(other.year) {
    dueDate = new char[strlen(other.dueDate) + 1];
    strcpy(dueDate, other.dueDate);
    paymentDate = other.paymentDate ? new char[strlen(other.paymentDate) + 1] : nullptr;
    if (other.paymentDate) strcpy(paymentDate, other.paymentDate);
}

Bill& Bill::operator=(const Bill& other) {
    if (this != &other) {
        billId = other.billId;
        studentId = other.studentId;
        amount = other.amount;
        lateFee = other.lateFee;
        isPaid = other.isPaid;
        month = other.month;
        year = other.year;
        delete[] dueDate;
        delete[] paymentDate;
        dueDate = new char[strlen(other.dueDate) + 1];
        strcpy(dueDate, other.dueDate);
        paymentDate = other.paymentDate ? new char[strlen(other.paymentDate) + 1] : nullptr;
        if (other.paymentDate) strcpy(paymentDate, other.paymentDate);
    }
    return *this;
}

void Bill::applyLateFee() {
    if (!isPaid) {
        lateFee = amount * 0.1;
        cout << "Late fee of Rs" << lateFee << " applied.\n";
    }
}

void Bill::payBill() {
    isPaid = true;
    time_t now = time(nullptr);
    char* date = ctime(&now);
    setPaymentDate(date);
    cout << "Bill paid. Total: Rs" << amount + lateFee << "\n";
}

void Bill::setPaymentDate(const char* date) {
    delete[] paymentDate;
    paymentDate = new char[strlen(date) + 1];
    strcpy(paymentDate, date);
}

void Bill::displayBill() {
    cout << "Bill ID:" << billId << ", Student ID:" << studentId << ", Amount:Rs" << amount 
         << ", Late Fee:Rs" << lateFee << ", Total:Rs" << amount + lateFee 
         << ", Due:" << dueDate << ", Status:" << (isPaid ? "Paid" : "Unpaid");
    if (isPaid && paymentDate) cout << ", Paid on:" << paymentDate;
    cout << ", Month:" << month << "/" << year << "\n";
}

bool Bill::operator>(const Bill& other) const {
    return (amount + lateFee) > (other.amount + other.lateFee);
}

Bill Bill::operator+(double additional) const {
    Bill temp = *this;
    temp.amount += additional;
    return temp;
}

// TransportPass implementation
TransportPass::TransportPass() : passId(0), studentId(0), routeId(0), bill(nullptr), isApproved(false) {
    applicationDate = time(nullptr);
}

TransportPass::TransportPass(int id, int studId, int rId) : passId(id), studentId(studId), routeId(rId), bill(nullptr), isApproved(false) {
    applicationDate = time(nullptr);
}

TransportPass::~TransportPass() { 
    delete bill; 
}

TransportPass::TransportPass(const TransportPass& other) : passId(other.passId), studentId(other.studentId), routeId(other.routeId), isApproved(other.isApproved), applicationDate(other.applicationDate), approvalDate(other.approvalDate) {
    bill = other.bill ? new Bill(*other.bill) : nullptr;
}

TransportPass& TransportPass::operator=(const TransportPass& other) {
    if (this != &other) {
        passId = other.passId;
        studentId = other.studentId;
        routeId = other.routeId;
        isApproved = other.isApproved;
        applicationDate = other.applicationDate;
        approvalDate = other.approvalDate;
        delete bill;
        bill = other.bill ? new Bill(*other.bill) : nullptr;
    }
    return *this;
}

void TransportPass::generateBill(double amount, const char* dueDate) {
    bill = new Bill(passId * 10, studentId, amount, dueDate);
}

void TransportPass::approvePass() {
    isApproved = true;
    approvalDate = time(nullptr);
}

void TransportPass::displayPass() {
    cout << "\n--- Transport Pass ---\n";
    cout << "Pass ID:" << passId << ", Student ID:" << studentId << ", Route ID:" << routeId 
         << ", Status:" << (isApproved ? "Approved" : "Pending") << "\n";
}

void TransportPass::displayApplicationInfo() {
    displayPass();
    cout << "Application Date: " << ctime(&applicationDate);
    if (isApproved) {
        cout << "Approval Date: " << ctime(&approvalDate);
    }
}

// TransportManager implementation
TransportManager::TransportManager() {
    loadNextId();
    loadData();
    cleanupInvalidPasses();
}

TransportManager::~TransportManager() {
    saveData();
    saveNextId();
}

void TransportManager::cleanupInvalidPasses() {
    DataContainer<TransportPass*> validPasses;
    TransportPass** data = passes.getData();
    
    for (int i = 0; i < passes.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getStudentId() != 0) {
            validPasses.add(data[i]);
        } else if (data[i] != nullptr) {
            delete data[i];
        }
    }
    
    passes.clear();
    TransportPass** validData = validPasses.getData();
    for (int i = 0; i < validPasses.getSize(); i++) {
        passes.add(validData[i]);
    }
}

void TransportManager::loadNextId() {
    ifstream file("nextid.txt");
    if (file.is_open()) {
        file >> nextId;
        file.close();
    } else {
        nextId = 100;
    }
}

void TransportManager::saveNextId() {
    ofstream file("nextid.txt");
    if (file.is_open()) {
        file << nextId;
        file.close();
    }
}

void TransportManager::addStudent(Student* student) {
    if (student != nullptr && student->getUserId() != 0) {
        students.add(student);
    }
}

void TransportManager::addAdmin(Admin* admin) {
    if (admin != nullptr && admin->getUserId() != 0) {
        admins.add(admin);
    }
}

void TransportManager::addDriver(Driver* driver) {
    if (driver != nullptr && driver->getUserId() != 0) {
        drivers.add(driver);
    }
}

Driver* TransportManager::findDriver(int id) {
    Driver** data = drivers.getData();
    for (int i = 0; i < drivers.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getUserId() == id) {
            return data[i];
        }
    }
    return nullptr;
}

void TransportManager::displayAllDrivers() {
    cout << "\n=== ALL DRIVERS ===\n";
    Driver** data = drivers.getData();
    for (int i = 0; i < drivers.getSize(); i++) {
        if (data[i] != nullptr) {
            data[i]->displayDriverInfo();
        }
    }
}

Admin* TransportManager::findAdmin(int id) {
    Admin** data = admins.getData();
    for (int i = 0; i < admins.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getUserId() == id) {
            return data[i];
        }
    }
    return nullptr;
}

Student* TransportManager::findStudent(int id) {
    Student** data = students.getData();
    for (int i = 0; i < students.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getUserId() == id) {
            return data[i];
        }
    }
    return nullptr;
}

bool TransportManager::loginUser(int id, const char* password) {
    Student* s = findStudent(id);
    if (s && s->authenticate(password)) {
        setColor(10);
        cout << "Student Login Successful!\n";
        setColor(15);
        return true;
    }
    Admin* a = findAdmin(id);
    if (a && a->authenticate(password)) {
        setColor(10);
        cout << "Admin Login Successful!\n";
        setColor(15);
        return true;
    }
    Driver* d = findDriver(id);
    if (d && d->authenticate(password)) {
        setColor(10);
        cout << "Driver Login Successful!\n";
        setColor(15);
        return true;
    }
    setColor(12);
    cout << "Invalid ID or Password!\n";
    setColor(15);
    return false;
}

void TransportManager::addVehicle(Vehicle* vehicle) {
    if (vehicle != nullptr && vehicle->getVehicleId() != 0) {
        vehicles.add(vehicle);
    }
}

void TransportManager::editVehicle(int id) {
    Vehicle* v = findVehicle(id);
    if (v) {
        char reg[20], model[30];
        int cap;
        cout << "New Registration: "; cin >> reg;
        cout << "New Model: "; cin >> model;
        cout << "New Capacity: "; cin >> cap;
        cout << "Vehicle updated successfully!\n";
    } else {
        cout << "Vehicle not found!\n";
    }
}

void TransportManager::removeVehicle(int id) {
    Vehicle** data = vehicles.getData();
    for (int i = 0; i < vehicles.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getVehicleId() == id) {
            delete data[i];
            vehicles.removeAt(i);
            cout << "Vehicle removed successfully!\n";
            return;
        }
    }
    cout << "Vehicle not found!\n";
}

void TransportManager::displayAllVehicles() {
    cout << "\n=== ALL VEHICLES ===\n";
    Vehicle** data = vehicles.getData();
    for (int i = 0; i < vehicles.getSize(); i++) {
        if (data[i] != nullptr) {
            data[i]->displayDetails();
        }
    }
}

void TransportManager::displayAvailableVehicles() {
    cout << "\n=== AVAILABLE VEHICLES ===\n";
    Vehicle** data = vehicles.getData();
    bool found = false;
    for (int i = 0; i < vehicles.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getAvailability()) {
            data[i]->displayDetails();
            found = true;
        }
    }
    if (!found) {
        cout << "No available vehicles found.\n";
    }
}

Vehicle* TransportManager::findVehicle(int id) {
    Vehicle** data = vehicles.getData();
    for (int i = 0; i < vehicles.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getVehicleId() == id) {
            return data[i];
        }
    }
    return nullptr;
}

void TransportManager::assignDriverToVehicle(int driverId, int vehicleId) {
    Driver* driver = findDriver(driverId);
    Vehicle* vehicle = findVehicle(vehicleId);
    if (driver && vehicle) {
        driver->setAssignedVehicleId(vehicleId);
        vehicle->setAssignedDriverId(driverId);
        //cout << "Driver assigned to vehicle successfully!\n";
    } else {
        //cout << "Driver or Vehicle not found!\n";
    }
}

void TransportManager::addRoute(Route* route) {
    if (route != nullptr && route->getRouteId() != 0) {
        routes.add(route);
    }
}

void TransportManager::assignVehicleToRoute(int routeId, int vehicleId) {
    Route* route = findRoute(routeId);
    Vehicle* vehicle = findVehicle(vehicleId);
    if (route && vehicle) {
        route->assignVehicle(vehicle);
    } else {
        cout << "Route or Vehicle not found!\n";
    }
}

void TransportManager::displayAllRoutes() {
    setColor(11);
    cout << "\n=== ALL ROUTES ===\n";
    setColor(15);
    Route** data = routes.getData();
    for (int i = 0; i < routes.getSize(); i++) {
        if (data[i] != nullptr) {
            data[i]->displayRoute();
        }
    }
}

Route* TransportManager::findRoute(int id) {
    Route** data = routes.getData();
    for (int i = 0; i < routes.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getRouteId() == id) {
            return data[i];
        }
    }
    return nullptr;
}

void TransportManager::applyForTransport(int studentId, int routeId) {
    Student* student = findStudent(studentId);
    Route* route = findRoute(routeId);
    if (student && route && route->isAvailable()) {
        student->applyForTransport(route, route->getAssignedVehicle());
        if (student->getTransportPass()) {
            passes.add(student->getTransportPass());
        }
    } else {
        cout << "Application failed. Route not available or already registered.\n";
    }
}

void TransportManager::viewAllApplications() {
    setColor(13);
    cout << "\n=== ALL APPLICATIONS ===\n";
    setColor(15);
    TransportPass** data = passes.getData();
    bool hasValidPasses = false;
    
    for (int i = 0; i < passes.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getStudentId() != 0) {
            data[i]->displayApplicationInfo();
            hasValidPasses = true;
        }
    }
    
    if (!hasValidPasses) {
        setColor(12);
        cout << "No pending applications found.\n";
        setColor(15);
    }
}

void TransportManager::approveApplication(int passId) {
    TransportPass** data = passes.getData();
    for (int i = 0; i < passes.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getPassId() == passId && data[i]->getStudentId() != 0) {
            data[i]->approvePass();
            if (data[i]->getBill()) {
                bills.add(data[i]->getBill());
            }
            setColor(10);
            cout << "Application approved successfully!\n";
            setColor(15);
            return;
        }
    }
    setColor(12);
    cout << "Application not found.\n";
    setColor(15);
}

void TransportManager::rejectApplication(int passId) {
    cout << "Application " << passId << " rejected.\n";
}

void TransportManager::cancelRegistration(int studentId) {
    Student* s = findStudent(studentId);
    if (s) {
        s->cancelRegistration();
    }
}

void TransportManager::generateInvoice(int studentId) {
    Student* s = findStudent(studentId);
    if (s && s->getTransportPass() && s->getTransportPass()->getBill()) {
        cout << "\n=== INVOICE ===\n";
        s->getTransportPass()->getBill()->displayBill();
    } else {
        cout << "No bill found for this student.\n";
    }
}

void TransportManager::generateMonthlyInvoice(int studentId, int month, int year) {
    Bill** data = bills.getData();
    cout << "\n=== MONTHLY INVOICE (" << month << "/" << year << ") ===\n";
    bool found = false;
    for (int i = 0; i < bills.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getStudentId() == studentId 
            && data[i]->getMonth() == month && data[i]->getYear() == year) {
            data[i]->displayBill();
            found = true;
        }
    }
    if (!found) {
        cout << "No invoice found for this month.\n";
    }
}

void TransportManager::applyLateFines() {
    Bill** data = bills.getData();
    cout << "\n=== APPLYING LATE FINES ===\n";
    for (int i = 0; i < bills.getSize(); i++) {
        if (data[i] != nullptr) {
            data[i]->applyLateFee();
        }
    }
}

void TransportManager::generateRevenueReport() {
    double total = 0;
    double totalPaid = 0;
    double totalPending = 0;
    Bill** data = bills.getData();
    cout << "\n=== REVENUE REPORT ===\n";
    for (int i = 0; i < bills.getSize(); i++) {
        if (data[i] != nullptr) {
            total += data[i]->getTotalAmount();
            data[i]->displayBill();
        }
    }
    cout << "\n--- SUMMARY ---\n";
    cout << "TOTAL REVENUE: Rs" << total << "\n";
}

void TransportManager::generateMonthlyRevenueReport(int month, int year) {
    double total = 0;
    Bill** data = bills.getData();
    cout << "\n=== MONTHLY REVENUE REPORT (" << month << "/" << year << ") ===\n";
    for (int i = 0; i < bills.getSize(); i++) {
        if (data[i] != nullptr && data[i]->getMonth() == month && data[i]->getYear() == year) {
            total += data[i]->getTotalAmount();
            data[i]->displayBill();
        }
    }
    cout << "\nTOTAL REVENUE for " << month << "/" << year << ": Rs" << total << "\n";
}

void TransportManager::generateRouteUsageReport() {
    cout << "\n=== ROUTE USAGE REPORT ===\n";
    Route** data = routes.getData();
    for (int i = 0; i < routes.getSize(); i++) {
        if (data[i] != nullptr) {
            cout << "Route " << data[i]->getRouteId() << " (" << data[i]->getStartPoint() << " -> " << data[i]->getEndPoint() << "): " 
                 << data[i]->getCurrentBookings() << " bookings, Capacity: ";
            if (data[i]->getAssignedVehicle()) {
                cout << data[i]->getAssignedVehicle()->getCapacity();
                double utilization = (double)data[i]->getCurrentBookings() / data[i]->getAssignedVehicle()->getCapacity() * 100;
                cout << " (Utilization: " << utilization << "%)";
            } else {
                cout << "No vehicle assigned";
            }
            cout << "\n";
        }
    }
}

void TransportManager::generateVehicleReport() {
    cout << "\n=== VEHICLE REPORT ===\n";
    Vehicle** data = vehicles.getData();
    double totalFuelCost = 0;
    double totalMaintenance = 0;
    for (int i = 0; i < vehicles.getSize(); i++) {
        if (data[i] != nullptr) {
            data[i]->displayDetails();
            cout << "   Monthly Fuel Cost (est): Rs" << data[i]->calculateFuelCost(500) << "\n";
            cout << "   Monthly Maintenance: Rs" << data[i]->calculateMaintenanceCost() << "\n";
            totalFuelCost += data[i]->calculateFuelCost(500);
            totalMaintenance += data[i]->calculateMaintenanceCost();
        }
    }
    cout << "\n--- VEHICLE SUMMARY ---\n";
    cout << "Total Estimated Fuel Cost: Rs" << totalFuelCost << "\n";
    cout << "Total Estimated Maintenance: Rs" << totalMaintenance << "\n";
    cout << "Total Operating Cost: Rs" << (totalFuelCost + totalMaintenance) << "\n";
}

void TransportManager::generateDriverReport() {
    cout << "\n=== DRIVER REPORT ===\n";
    Driver** data = drivers.getData();
    double totalSalary = 0;
    for (int i = 0; i < drivers.getSize(); i++) {
        if (data[i] != nullptr) {
            data[i]->displayDriverInfo();
            totalSalary += data[i]->getSalary();
        }
    }
    cout << "\nTotal Monthly Salary Expense: Rs" << totalSalary << "\n";
}

void TransportManager::saveData() {
    ofstream file("TMS_data.txt");
    if (file.is_open()) {
        // SAVE STUDENTS
        int studentCount = 0;
        Student** sdata = students.getData();
        for (int i = 0; i < students.getSize(); i++) {
            if (sdata[i] != nullptr && sdata[i]->getUserId() != 0) {
                studentCount++;
            }
        }
        file << studentCount << endl;
        
        for (int i = 0; i < students.getSize(); i++) {
            if (sdata[i] != nullptr && sdata[i]->getUserId() != 0) {
                file << sdata[i]->getUserId() << endl;
                file << sdata[i]->getUsername() << endl;
                file << sdata[i]->getPassword() << endl;
                file << sdata[i]->getEmail() << endl;
                file << sdata[i]->getRollNo() << endl;
                file << sdata[i]->getDepartment() << endl;
                file << sdata[i]->hasPass() << endl;
            }
        }
        
        // SAVE DRIVERS
        int driverCount = 0;
        Driver** ddata = drivers.getData();
        for (int i = 0; i < drivers.getSize(); i++) {
            if (ddata[i] != nullptr && ddata[i]->getUserId() != 0) {
                driverCount++;
            }
        }
        file << driverCount << endl;
        
        for (int i = 0; i < drivers.getSize(); i++) {
            if (ddata[i] != nullptr && ddata[i]->getUserId() != 0) {
                file << ddata[i]->getUserId() << endl;
                file << ddata[i]->getUsername() << endl;
                file << ddata[i]->getPassword() << endl;
                file << ddata[i]->getEmail() << endl;
                file << ddata[i]->getLicenseNumber() << endl;
                file << ddata[i]->getPhoneNumber() << endl;
                file << ddata[i]->getSalary() << endl;
                file << ddata[i]->getAssignedVehicleId() << endl;
            }
        }
        
        // SAVE PASSES
        int passCount = 0;
        TransportPass** pdata = passes.getData();
        for (int i = 0; i < passes.getSize(); i++) {
            if (pdata[i] != nullptr && pdata[i]->getStudentId() != 0) {
                passCount++;
            }
        }
        file << passCount << endl;
        
        for (int i = 0; i < passes.getSize(); i++) {
            if (pdata[i] != nullptr && pdata[i]->getStudentId() != 0) {
                file << pdata[i]->getPassId() << endl;
                file << pdata[i]->getStudentId() << endl;
                file << pdata[i]->getRouteId() << endl;
                file << pdata[i]->getApprovalStatus() << endl;
                
                if (pdata[i]->getBill()) {
                    file << 1 << endl;
                    file << pdata[i]->getBill()->getBillId() << endl;
                    file << pdata[i]->getBill()->getStudentId() << endl;
                    file << pdata[i]->getBill()->getTotalAmount() - pdata[i]->getBill()->getTotalAmount()*0.1 << endl;
                    file << pdata[i]->getBill()->getTotalAmount()*0.1 << endl;
                } else {
                    file << 0 << endl;
                }
            }
        }
        
        // SAVE BILLS
        int billCount = 0;
        Bill** bdata = bills.getData();
        for (int i = 0; i < bills.getSize(); i++) {
            if (bdata[i] != nullptr && bdata[i]->getStudentId() != 0) {
                billCount++;
            }
        }
        file << billCount << endl;
        
        for (int i = 0; i < bills.getSize(); i++) {
            if (bdata[i] != nullptr && bdata[i]->getStudentId() != 0) {
                file << bdata[i]->getBillId() << endl;
                file << bdata[i]->getStudentId() << endl;
                file << bdata[i]->getTotalAmount() - bdata[i]->getTotalAmount()*0.1 << endl;
                file << bdata[i]->getTotalAmount()*0.1 << endl;
                file << bdata[i]->getMonth() << endl;
                file << bdata[i]->getYear() << endl;
            }
        }
        
        // SAVE ROUTE BOOKINGS
        int routeCount = 0;
        Route** rdata = routes.getData();
        for (int i = 0; i < routes.getSize(); i++) {
            if (rdata[i] != nullptr && rdata[i]->getRouteId() != 0) {
                routeCount++;
            }
        }
        file << routeCount << endl;
        
        for (int i = 0; i < routes.getSize(); i++) {
            if (rdata[i] != nullptr && rdata[i]->getRouteId() != 0) {
                file << rdata[i]->getRouteId() << endl;
                file << rdata[i]->getCurrentBookings() << endl;
            }
        }
        
        file.close();
        cout << "Data saved successfully!\n";
    }
}

void TransportManager::loadData() {
    ifstream file("TMS_data.txt");
    if (file.is_open()) {
        int count;
        // LOAD STUDENTS
        file >> count;
        int maxId = 0;
        
        for (int i = 0; i < count; i++) {
            int id;
            int roll;
            char name[50];
            char pass[50];
            char email[50];
            char dept[50];
            bool hasPass;
            
            file >> id;
            file >> name;
            file >> pass;
            file >> email;
            file >> roll;
            file >> dept;
            file >> hasPass;
            
            if (id > maxId) maxId = id;
            if (id != 0 && strlen(name) > 0) {
                Student* student = new Student(id, name, pass, email, roll, dept);
                student->setHasActivePass(hasPass);
                addStudent(student);
            }
        }
        
        // LOAD DRIVERS
        file >> count;
        for (int i = 0; i < count; i++) {
            int id, assignedVehicleId;
            char name[50], pass[50], email[50], license[50], phone[50];
            double salary;
            
            file >> id;
            file >> name;
            file >> pass;
            file >> email;
            file >> license;
            file >> phone;
            file >> salary;
            file >> assignedVehicleId;
            
            if (id > maxId) maxId = id;
            if (id != 0 && strlen(name) > 0) {
                Driver* driver = new Driver(id, name, pass, email, license, phone, salary);
                driver->setAssignedVehicleId(assignedVehicleId);
                addDriver(driver);
            }
        }
        
        // LOAD PASSES
        int passCount;
        file >> passCount;
        for (int i = 0; i < passCount; i++) {
            int passId, studentId, routeId;
            bool approved;
            int hasBill;
            
            file >> passId >> studentId >> routeId >> approved;
            file >> hasBill;
            
            if (studentId != 0 && passId != 0) {
                TransportPass* pass = new TransportPass(passId, studentId, routeId);
                if (approved) {
                    pass->approvePass();
                }
                
                if (hasBill) {
                    int billId, studId;
                    double amount, lateFee;
                    file >> billId >> studId >> amount >> lateFee;
                    Bill* bill = new Bill(billId, studId, amount, "2026-06-10");
                    bill->setLateFee(lateFee);
                    pass->setBill(bill);
                }
                
                passes.add(pass);
                
                Student* student = findStudent(studentId);
                if (student) {
                    student->setTransportPassDirectly(pass);
                }
            } else {
                if (hasBill) {
                    int tempId1, tempId2;
                    double temp1, temp2;
                    file >> tempId1 >> tempId2 >> temp1 >> temp2;
                }
            }
        }
        
        // LOAD BILLS
        int billCount;
        file >> billCount;
        for (int i = 0; i < billCount; i++) {
            int billId, studentId, month, year;
            double amount, lateFee;
            file >> billId >> studentId >> amount >> lateFee >> month >> year;
            if (studentId != 0 && billId != 0) {
                Bill* bill = new Bill(billId, studentId, amount, "2026-06-10");
                bill->setLateFee(lateFee);
                bills.add(bill);
            }
        }
        
        // LOAD ROUTE BOOKINGS
        int routeCount;
        file >> routeCount;
        for (int i = 0; i < routeCount; i++) {
            int routeId, bookings;
            file >> routeId >> bookings;
            Route* route = findRoute(routeId);
            if (route && bookings > 0) {
                for (int j = 0; j < bookings; j++) {
                    route->addBooking();
                }
            }
        }
        
        nextId = maxId + 1;
        file.close();
       // cout << "Data loaded successfully!\n";
    }
}

void TransportManager::displayStudentMenu(Student* student) {
    int choice;
    do {
        student->displayMenu();
        cin >> choice;
        switch(choice) {
            case 1: 
                displayAllRoutes(); 
                break;
            case 2: {
                int routeId;
                cout << "Enter Route ID: ";
                cin >> routeId;
                applyForTransport(student->getUserId(), routeId);
                break;
            }
            case 3: 
                student->viewRegistration(); 
                break;
            case 4: 
                cancelRegistration(student->getUserId()); 
                break;
            case 5: {
                if (student->getTransportPass() && student->getTransportPass()->getBill()) {
                    student->getTransportPass()->getBill()->payBill();
                } else {
                    cout << "No bill found!\n";
                }
                break;
            }
            case 6:
                generateInvoice(student->getUserId());
                break;
            case 7: 
                cout << "Logging out...\n"; 
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while(choice != 7);
}

void TransportManager::displayAdminMenu(Admin* admin) {
    int choice;
    do {
        admin->displayMenu();
        cin >> choice;
        switch(choice) {
            case 1: {
                int type, id = getNextId();
                char reg[20], model[30];
                int cap;
                cout << "Vehicle Type (1-Bus, 2-Van): ";
                cin >> type;
                cout << "Registration Number: "; cin >> reg;
                cout << "Model: "; cin >> model;
                cout << "Capacity: "; cin >> cap;
                if (type == 1) {
                    addVehicle(new Bus(id, reg, model, cap, 100, true));
                } else {
                    addVehicle(new Van(id, reg, model, cap, 200, 12));
                }
                cout << "Vehicle added successfully!\n";
                break;
            }
            case 2: {
                int vid;
                cout << "Enter Vehicle ID to edit: ";
                cin >> vid;
                editVehicle(vid);
                break;
            }
            case 3: {
                int rid;
                cout << "Enter Vehicle ID to remove: ";
                cin >> rid;
                removeVehicle(rid);
                break;
            }
            case 4:
                displayAvailableVehicles();
                break;
            case 5: {
                int id = getNextId();
                char start[50], end[50];
                double dist;
                cout << "Start Point: "; cin >> start;
                cout << "End Point: "; cin >> end;
                cout << "Distance (km): "; cin >> dist;
                addRoute(new Route(id, start, end, dist));
                cout << "Route added successfully!\n";
                break;
            }
            case 6: {
                int routeId, vehicleId;
                displayAllRoutes();
                cout << "Enter Route ID: "; cin >> routeId;
                displayAllVehicles();
                cout << "Enter Vehicle ID: "; cin >> vehicleId;
                assignVehicleToRoute(routeId, vehicleId);
                break;
            }
            case 7: {
                int id = getNextId();
                char name[50], pass[50], email[50], license[50], phone[50];
                double salary;
                cout << "Driver Name: "; cin >> name;
                cout << "Password: "; cin >> pass;
                cout << "Email: "; cin >> email;
                cout << "License Number: "; cin >> license;
                cout << "Phone Number: "; cin >> phone;
                cout << "Salary: "; cin >> salary;
                addDriver(new Driver(id, name, pass, email, license, phone, salary));
                setColor(10);
                cout << "Driver added successfully! ID: " << id << "\n";
                 setColor(15);
                break;
            }
            case 8: {
                int driverId, vehicleId;
                displayAllDrivers();
                cout << "Enter Driver ID: "; cin >> driverId;
                displayAllVehicles();
                cout << "Enter Vehicle ID: "; cin >> vehicleId;
                assignDriverToVehicle(driverId, vehicleId);
                break;
            }
            case 9:
                viewAllApplications();
                break;
            case 10: {
                int passId;
                cout << "Enter Pass ID to approve: ";
                cin >> passId;
                approveApplication(passId);
                break;
            }
            case 11: {
                int reportChoice;
                cout << "\n========== REPORTS ==========\n";
                cout << "1. Revenue Report\n";
                cout << "2. Monthly Revenue Report\n";
                cout << "3. Route Usage Report\n";
                cout << "4. Vehicle Report\n";
                cout << "5. Driver Report\n";
                cout << "Choice: ";
                cin >> reportChoice;
                if (reportChoice == 1) generateRevenueReport();
                else if (reportChoice == 2) {
                    int month, year;
                    cout << "Enter Month (1-12): "; cin >> month;
                    cout << "Enter Year: "; cin >> year;
                    generateMonthlyRevenueReport(month, year);
                }
                else if (reportChoice == 3) generateRouteUsageReport();
                else if (reportChoice == 4) generateVehicleReport();
                else if (reportChoice == 5) generateDriverReport();
                else cout << "Invalid choice!\n";
                break;
            }
            case 12:
                applyLateFines();
                break;
            case 13:
                saveData();
                break;
            case 14:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while(choice != 14);
}

void TransportManager::displayDriverMenu(Driver* driver) {
    int choice;
    do {
        driver->displayMenu();
        cin >> choice;
        switch(choice) {
            case 1: {
                int vehicleId = driver->getAssignedVehicleId();
                if (vehicleId != -1) {
                    Vehicle* v = findVehicle(vehicleId);
                    if (v) {
                        cout << "\n=== ASSIGNED VEHICLE ===\n";
                        v->displayDetails();
                    }
                } else {
                    cout << "No vehicle assigned yet.\n";
                }
                break;
            }
            case 2: {
                int vehicleId = driver->getAssignedVehicleId();
                if (vehicleId != -1) {
                    Vehicle* v = findVehicle(vehicleId);
                    if (v) {
                        v->setAvailability(!v->getAvailability());
                        cout << "Vehicle availability updated to: " << (v->getAvailability() ? "Available" : "Not Available") << "\n";
                    }
                } else {
                    cout << "No vehicle assigned.\n";
                }
                break;
            }
            case 3:
                cout << "\n=== MY SCHEDULE ===\n";
                cout << "Driver: " << driver->getUsername() << "\n";
                cout << "Working Hours: 8:00 AM - 4:00 PM\n";
                cout << "Days: Monday to Saturday\n";
                break;
            case 4:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while(choice != 4);
}

// Main function
int main() {
    TransportManager manager;
    
    // Add default admin
    Admin* admin1 = new Admin(1, "admin", "admin123", "admin@gmail.com");
    manager.addAdmin(admin1);
    
    // Add default drivers
    manager.addDriver(new Driver(10, "John Doe", "driver123", "john@example.com", "LIC-001", "03001234567", 50000));
    manager.addDriver(new Driver(11, "Jane Smith", "driver123", "jane@example.com", "LIC-002", "03007654321", 45000));
    
    // Add default vehicles
    manager.addVehicle(new Bus(1, "LEH-123", "Toyota Hiace", 40, 101, true));
    manager.addVehicle(new Bus(2, "LEH-456", "Hino Rainbow", 50, 102, false));
    manager.addVehicle(new Van(3, "LEH-789", "Suzuki Carry", 12, 201, 10));
    manager.addVehicle(new Van(4, "LEH-999", "Daihatsu Hijet", 10, 202, 8));
    
    // Assign drivers to vehicles
    manager.assignDriverToVehicle(10, 1);
    manager.assignDriverToVehicle(11, 2);
    
    // Add default routes
    manager.addRoute(new Route(1, "Main Campus", "City Center", 15.5));
    manager.addRoute(new Route(2, "Main Campus", "University Town", 8.2));
    manager.addRoute(new Route(3, "Main Campus", "Garden Town", 12.0));
    manager.addRoute(new Route(4, "Main Campus", "Model Town", 18.5));
    
    // Assign vehicles to routes
    manager.assignVehicleToRoute(1, 1);
    manager.assignVehicleToRoute(2, 2);
    manager.assignVehicleToRoute(3, 3);
    manager.assignVehicleToRoute(4, 4);
    
    int choice, userId;
    char password[50];
    
    do {
        setColor(11);
        cout << "\n========================================\n";
        cout << "   UNIVERSITY TRANSPORT MANAGEMENT SYSTEM";
        cout << "\n========================================\n";
        setColor(15);
        cout << "1. Register New User\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        setColor(14);
        cout << "Enter choice: ";
        setColor(15);
        cin >> choice;
        
        if (choice == 1) {
            int roll;
            char name[50], pass[50], email[50], dept[50];
            setColor(11);
            cout << "\n=== NEW USER REGISTRATION ===\n";
            setColor(15);
            cout << "Full Name: "; cin >> name;
            cout << "Password: "; cin >> pass;
            cout << "Email: "; cin >> email;
            cout << "Roll Number: "; cin >> roll;
            cout << "Department: "; cin >> dept;
            
            int newId = manager.getNextId();
            manager.addStudent(new Student(newId, name, pass, email, roll, dept));
            setColor(10);
            cout << "\nRegistration successful!" << endl;
            cout << "Your User ID is: " << newId << endl;
            setColor(15);
        }
        else if (choice == 2) {
            setColor(11);
            cout << "\n=== LOGIN ===\n";
            setColor(15);
            cout << "User ID: "; cin >> userId;
            cout << "Password: "; cin >> password;
            
            if (userId == 1 && strcmp(password, "admin123") == 0) {
                Admin* admin = manager.findAdmin(1);
                if (admin) {
                    manager.displayAdminMenu(admin);
                }
            }
            else if (manager.loginUser(userId, password)) {
                Student* student = manager.findStudent(userId);
                if (student) {
                    setColor(10);
                    cout << "\nWelcome, " << student->getUsername() << "!\n";
                    setColor(15);
                    manager.displayStudentMenu(student);
                } else {
                    Driver* driver = manager.findDriver(userId);
                    if (driver) {
                        setColor(10);
                        cout << "\nWelcome, " << driver->getUsername() << "!\n";
                        setColor(15);
                        manager.displayDriverMenu(driver);
                    }
                }
            }
            else {
                setColor(12);
                cout << "\nInvalid credentials! Please try again.\n";
                setColor(15);
            }
        }
        else if (choice == 3) {
            setColor(14);
            cout << "\nThank you for using University Transport System!\n";
            cout << "Goodbye!\n";
            setColor(15);
        }
        else {
            setColor(12);
            cout << "\nInvalid choice! Please try again.\n";
            setColor(15);
        }
    } while (choice != 3);
    
    return 0;
}