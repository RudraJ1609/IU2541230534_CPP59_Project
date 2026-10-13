#ifndef PROPERTY_H
#define PROPERTY_H
#include <string>

// Base class: a rental property
class Property {
protected:
    int id;
    std::string address;
    double rent;      // monthly rent
    int ownerId;
    bool rented;
public:
    Property();                                              // default constructor
    Property(int id, std::string address, double rent, int ownerId, bool rented = false);
    virtual ~Property();                                     // virtual destructor

    int getId() const;
    std::string getAddress() const;
    double getRent() const;
    int getOwnerId() const;
    bool isRented() const;

    void setAddress(std::string address);
    void setRent(double rent);
    void setRented(bool rented);

    virtual std::string getType() const;     // overridden in derived class
    virtual void display() const;            // overridden in derived class
    virtual std::string toLine() const;      // text line for the data file
};
#endif
