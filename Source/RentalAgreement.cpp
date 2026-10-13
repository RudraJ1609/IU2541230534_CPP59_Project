#include "RentalAgreement.h"
#include <iostream>
#include <stdexcept>
using namespace std;

RentalAgreement::RentalAgreement() {
    id = 0; propertyId = 0; tenantId = 0; ownerId = 0; months = 0; monthlyRent = 0;
}

RentalAgreement::RentalAgreement(int id, int propertyId, int tenantId, int ownerId,
                                 int months, double monthlyRent) {
    if (id <= 0) throw invalid_argument("Agreement ID must be positive.");
    if (months < 1 || months > 120) throw invalid_argument("Months must be between 1 and 120.");
    if (monthlyRent <= 0) throw invalid_argument("Rent must be greater than 0.");
    this->id = id;
    this->propertyId = propertyId;
    this->tenantId = tenantId;
    this->ownerId = ownerId;
    this->months = months;
    this->monthlyRent = monthlyRent;
}

RentalAgreement::~RentalAgreement() {}

int RentalAgreement::getId() const { return id; }
int RentalAgreement::getPropertyId() const { return propertyId; }
int RentalAgreement::getTenantId() const { return tenantId; }
int RentalAgreement::getOwnerId() const { return ownerId; }
int RentalAgreement::getMonths() const { return months; }
double RentalAgreement::getMonthlyRent() const { return monthlyRent; }
double RentalAgreement::getTotal() const { return months * monthlyRent; }

void RentalAgreement::display() const {
    cout << "Agreement " << id << " | Property " << propertyId << " | Tenant " << tenantId
         << " | Owner " << ownerId << " | " << months << " months x " << monthlyRent
         << " = " << getTotal() << "\n";
}

string RentalAgreement::toLine() const {
    return to_string(id) + "|" + to_string(propertyId) + "|" + to_string(tenantId) + "|" +
           to_string(ownerId) + "|" + to_string(months) + "|" + to_string(monthlyRent);
}
