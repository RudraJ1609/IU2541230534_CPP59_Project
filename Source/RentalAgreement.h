#ifndef RENTALAGREEMENT_H
#define RENTALAGREEMENT_H
#include <string>

// Links one Property, one Tenant and one Owner (by their IDs)
class RentalAgreement {
private:
    int id;
    int propertyId;
    int tenantId;
    int ownerId;
    int months;
    double monthlyRent;
public:
    RentalAgreement();
    RentalAgreement(int id, int propertyId, int tenantId, int ownerId,
                    int months, double monthlyRent);
    ~RentalAgreement();

    int getId() const;
    int getPropertyId() const;
    int getTenantId() const;
    int getOwnerId() const;
    int getMonths() const;
    double getMonthlyRent() const;
    double getTotal() const;          // months * monthlyRent

    void display() const;
    std::string toLine() const;
};
#endif
