#include "ResidentialProperty.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace std;

ResidentialProperty::ResidentialProperty() : Property() { bedrooms = 1; }

ResidentialProperty::ResidentialProperty(int id, string address, double rent, int ownerId,
                                         int bedrooms, bool rented)
    : Property(id, address, rent, ownerId, rented) {
    setBedrooms(bedrooms);
}

int ResidentialProperty::getBedrooms() const { return bedrooms; }

void ResidentialProperty::setBedrooms(int bedrooms) {
    if (bedrooms < 1 || bedrooms > 10)
        throw invalid_argument("Bedrooms must be between 1 and 10.");
    this->bedrooms = bedrooms;
}

string ResidentialProperty::getType() const { return "Residential"; }

void ResidentialProperty::display() const {
    Property::display();   // reuse the base class output
    cout << "   Type: " << getType() << " | Bedrooms: " << bedrooms << "\n";
}

string ResidentialProperty::toLine() const {
    ostringstream out;
    out << "R|" << id << "|" << address << "|" << rent << "|" << ownerId << "|" << rented
        << "|" << bedrooms;
    return out.str();
}
