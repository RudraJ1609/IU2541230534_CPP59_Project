#include "Property.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace std;

Property::Property() {
    id = 0; address = ""; rent = 0; ownerId = 0; rented = false;
}

Property::Property(int id, string address, double rent, int ownerId, bool rented) {
    if (id <= 0) throw invalid_argument("Property ID must be positive.");
    this->id = id;
    this->ownerId = ownerId;
    this->rented = rented;
    setAddress(address);   // setters validate the data
    setRent(rent);
}

Property::~Property() {}

int Property::getId() const { return id; }
string Property::getAddress() const { return address; }
double Property::getRent() const { return rent; }
int Property::getOwnerId() const { return ownerId; }
bool Property::isRented() const { return rented; }

void Property::setAddress(string address) {
    if (address.empty() || address.find('|') != string::npos)
        throw invalid_argument("Address cannot be empty or contain '|'.");
    this->address = address;
}

void Property::setRent(double rent) {
    if (rent <= 0) throw invalid_argument("Rent must be greater than 0.");
    this->rent = rent;
}

void Property::setRented(bool rented) { this->rented = rented; }

string Property::getType() const { return "Property"; }

void Property::display() const {
    cout << "Property " << id << " | " << address << " | Rent: " << rent
         << " | Owner ID: " << ownerId << " | " << (rented ? "Rented" : "Available") << "\n";
}

string Property::toLine() const {
    ostringstream out;
    out << "P|" << id << "|" << address << "|" << rent << "|" << ownerId << "|" << rented;
    return out.str();
}
