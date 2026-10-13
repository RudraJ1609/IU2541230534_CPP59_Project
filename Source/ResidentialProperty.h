#ifndef RESIDENTIALPROPERTY_H
#define RESIDENTIALPROPERTY_H
#include "Property.h"

// Derived class: house or flat with bedrooms
class ResidentialProperty : public Property {
private:
    int bedrooms;
public:
    ResidentialProperty();
    ResidentialProperty(int id, std::string address, double rent, int ownerId,
                        int bedrooms, bool rented = false);

    int getBedrooms() const;
    void setBedrooms(int bedrooms);

    std::string getType() const;      // overrides Property::getType
    void display() const;             // overrides Property::display
    std::string toLine() const;       // overrides Property::toLine
};
#endif
