#ifndef PROPERTYSYSTEM_H
#define PROPERTYSYSTEM_H
#include "Property.h"
#include "ResidentialProperty.h"
#include "Tenant.h"
#include "Owner.h"
#include "RentalAgreement.h"
#include "FileManager.h"

// Manages all records using simple dynamic arrays
class PropertySystem {
private:
    Property** properties;        // array of base pointers (polymorphism)
    int propCount, propCap;
    Tenant* tenants;
    int tenantCount, tenantCap;
    Owner* owners;
    int ownerCount, ownerCap;
    RentalAgreement* agreements;
    int agrCount, agrCap;
    int nextPropId, nextTenantId, nextOwnerId, nextAgrId;
    FileManager files;

    void initMemory();
    void freeMemory();
    void growProperties();
    void growTenants();
    void growOwners();
    void growAgreements();

    int findProperty(int id) const;
    int findTenant(int id) const;
    int findOwner(int id) const;
    bool tenantInUse(int id) const;
    bool ownerInUse(int id) const;
    int chooseType() const;

    void addProperty();
    void addTenant();
    void addOwner();
    void updateProperty();
    void updateTenant();
    void updateOwner();
    void deleteProperty();
    void deleteTenant();
    void deleteOwner();

    void showMenu() const;
    void saveData();
    void loadData();
public:
    PropertySystem();
    ~PropertySystem();

    void run();                  // menu loop
    void addRecord();
    void displayRecords() const;
    void searchRecord() const;
    void updateRecord();
    void deleteRecord();
    void createAgreement();      // main transaction
    void showReport() const;
    void saveLoadMenu();
};
#endif
