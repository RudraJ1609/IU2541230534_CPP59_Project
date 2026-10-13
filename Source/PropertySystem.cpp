#include "PropertySystem.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <limits>  // For numeric_limits
using namespace std;

const int START_CAPACITY = 4;
const string PROPERTY_FILE = "Data/properties.txt";
const string TENANT_FILE = "Data/tenants.txt";
const string OWNER_FILE = "Data/owners.txt";
const string AGREEMENT_FILE = "Data/agreements.txt";

// ---------- simple input helpers ----------
static string readLine(const string& prompt) {
    string s;
    cout << prompt;
    // Check if the input stream is good before reading
    if (cin.fail()) {
        cout << "\nInput stream error. Please reset the input stream.\n";
        cin.clear();  // Clear the error state
        // Reset to a clean state for the user
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    if (!getline(cin, s)) { 
        // Check if it's EOF or an actual error
        if (cin.eof()) {
            cout << "\nEnd of input (Ctrl+D/Ctrl+Z). Exiting program.\n";
        } else {
            cout << "\nInput error. Exiting program.\n";
        }
        std::exit(0); 
    }
    
    // Trim whitespace (optional - removes leading/trailing spaces)
    // This helps with user input like "  42  "
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start != string::npos && end != string::npos) {
        s = s.substr(start, end - start + 1);
    }
    
    // Handle empty input after trimming
    if (s.empty()) {
        cout << "Input cannot be empty. Please enter a value:\n";
        // Recursively call to allow retry
        return readLine(prompt);
    }
    
    return s;
}

static int readInt(const string& prompt) {
    int maxAttempts = 10;
    int attempts = 0;
    
    while (attempts < maxAttempts) {
        string s = readLine(prompt);
        try {
            size_t pos = 0;
            int v = stoi(s, &pos);
            // Check that the entire string was consumed (no trailing characters)
            if (pos == s.size()) {
                return v;
            } else {
                cout << "Error: Invalid characters after number.\nPlease enter a valid whole number:\n";
            }
        } catch (const invalid_argument&) {
            cout << "Error: Not a valid integer.\nPlease enter a whole number:\n";
        } catch (const out_of_range&) {
            cout << "Error: Number out of range.\nPlease enter a smaller integer:\n";
        } catch (...) {
            cout << "Error: Invalid input.\nPlease enter a whole number:\n";
        }
        attempts++;
    }
    
    // If we reach here, too many failed attempts
    cout << "Too many failed attempts. Exiting program.\n";
    std::exit(0);
}

static double readDouble(const string& prompt) {
    int maxAttempts = 10;
    int attempts = 0;
    
    while (attempts < maxAttempts) {
        string s = readLine(prompt);
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            // Check that the entire string was consumed (no trailing characters)
            if (pos == s.size()) {
                return v;
            } else {
                cout << "Error: Invalid characters after number.\nPlease enter a valid number:\n";
            }
        } catch (const invalid_argument&) {
            cout << "Error: Not a valid number.\nPlease enter a number:\n";
        } catch (const out_of_range&) {
            cout << "Error: Number out of range.\nPlease enter a smaller number:\n";
        } catch (...) {
            cout << "Error: Invalid input.\nPlease enter a number:\n";
        }
        attempts++;
    }
    
    // If we reach here, too many failed attempts
    cout << "Too many failed attempts. Exiting program.\n";
    std::exit(0);
}

// ---------- memory management ----------
PropertySystem::PropertySystem() { initMemory(); }
PropertySystem::~PropertySystem() { freeMemory(); }

void PropertySystem::initMemory() {
    propCap = tenantCap = ownerCap = agrCap = START_CAPACITY;
    propCount = tenantCount = ownerCount = agrCount = 0;
    nextPropId = nextTenantId = nextOwnerId = nextAgrId = 1;
    properties = new Property*[propCap];
    tenants = new Tenant[tenantCap];
    owners = new Owner[ownerCap];
    agreements = new RentalAgreement[agrCap];
}

void PropertySystem::freeMemory() {
    for (int i = 0; i < propCount; i++) delete properties[i];   // delete each object
    delete[] properties;                                         // then the array
    delete[] tenants;
    delete[] owners;
    delete[] agreements;
}

void PropertySystem::growProperties() {
    int newCap = propCap * 2;
    Property** bigger = new Property*[newCap];
    for (int i = 0; i < propCount; i++) bigger[i] = properties[i];
    delete[] properties;
    properties = bigger;
    propCap = newCap;
}

void PropertySystem::growTenants() {
    int newCap = tenantCap * 2;
    Tenant* bigger = new Tenant[newCap];
    for (int i = 0; i < tenantCount; i++) bigger[i] = tenants[i];
    delete[] tenants;
    tenants = bigger;
    tenantCap = newCap;
}

void PropertySystem::growOwners() {
    int newCap = ownerCap * 2;
    Owner* bigger = new Owner[newCap];
    for (int i = 0; i < ownerCount; i++) bigger[i] = owners[i];
    delete[] owners;
    owners = bigger;
    ownerCap = newCap;
}

void PropertySystem::growAgreements() {
    int newCap = agrCap * 2;
    RentalAgreement* bigger = new RentalAgreement[newCap];
    for (int i = 0; i < agrCount; i++) bigger[i] = agreements[i];
    delete[] agreements;
    agreements = bigger;
    agrCap = newCap;
}

// ---------- find helpers (return index or -1) ----------
int PropertySystem::findProperty(int id) const {
    for (int i = 0; i < propCount; i++) if (properties[i]->getId() == id) return i;
    return -1;
}
int PropertySystem::findTenant(int id) const {
    for (int i = 0; i < tenantCount; i++) if (tenants[i].getId() == id) return i;
    return -1;
}
int PropertySystem::findOwner(int id) const {
    for (int i = 0; i < ownerCount; i++) if (owners[i].getId() == id) return i;
    return -1;
}
bool PropertySystem::tenantInUse(int id) const {
    for (int i = 0; i < agrCount; i++) if (agreements[i].getTenantId() == id) return true;
    return false;
}
bool PropertySystem::ownerInUse(int id) const {
    for (int i = 0; i < propCount; i++) if (properties[i]->getOwnerId() == id) return true;
    return false;
}

int PropertySystem::chooseType() const {
    cout << "1. Property  2. Tenant  3. Owner\n";
    int t = readInt("Choose record type: ");
    if (t < 1 || t > 3) { cout << "Invalid type.\n"; return 0; }
    return t;
}

// ---------- 1. Add ----------
void PropertySystem::addProperty() {
    if (ownerCount == 0) { cout << "Add an owner first.\n"; return; }
    int ownerId = readInt("Owner ID: ");
    if (findOwner(ownerId) < 0) { cout << "Owner not found.\n"; return; }
    string address = readLine("Address: ");
    double rent = readDouble("Monthly rent: ");
    int beds = readInt("Bedrooms: ");
    try {
        Property* p = new ResidentialProperty(nextPropId, address, rent, ownerId, beds);
        if (propCount == propCap) growProperties();
        properties[propCount++] = p;
        cout << "Property added with ID " << nextPropId++ << ".\n";
    } catch (invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void PropertySystem::addTenant() {
    string name = readLine("Tenant name: ");
    string phone = readLine("Phone (10 digits): ");
    try {
        Tenant t(nextTenantId, name, phone);
        if (tenantCount == tenantCap) growTenants();
        tenants[tenantCount++] = t;
        cout << "Tenant added with ID " << nextTenantId++ << ".\n";
    } catch (invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void PropertySystem::addOwner() {
    string name = readLine("Owner name: ");
    string phone = readLine("Phone (10 digits): ");
    try {
        Owner o(nextOwnerId, name, phone);
        if (ownerCount == ownerCap) growOwners();
        owners[ownerCount++] = o;
        cout << "Owner added with ID " << nextOwnerId++ << ".\n";
    } catch (invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void PropertySystem::addRecord() {
    int t = chooseType();
    if (t == 1) addProperty();
    else if (t == 2) addTenant();
    else if (t == 3) addOwner();
}

// ---------- 2. Display ----------
void PropertySystem::displayRecords() const {
    cout << "\n--- Properties (" << propCount << ") ---\n";
    for (int i = 0; i < propCount; i++) properties[i]->display();   // runtime polymorphism
    cout << "--- Tenants (" << tenantCount << ") ---\n";
    for (int i = 0; i < tenantCount; i++) tenants[i].display();
    cout << "--- Owners (" << ownerCount << ") ---\n";
    for (int i = 0; i < ownerCount; i++) owners[i].display();
    cout << "--- Agreements (" << agrCount << ") ---\n";
    for (int i = 0; i < agrCount; i++) agreements[i].display();
}

// ---------- 3. Search ----------
void PropertySystem::searchRecord() const {
    int t = chooseType();
    if (t == 0) return;
    int id = readInt("Enter ID to search: ");
    if (t == 1 && findProperty(id) >= 0) properties[findProperty(id)]->display();
    else if (t == 2 && findTenant(id) >= 0) tenants[findTenant(id)].display();
    else if (t == 3 && findOwner(id) >= 0) owners[findOwner(id)].display();
    else cout << "Record not found.\n";
}

// ---------- 4. Update ----------
void PropertySystem::updateProperty() {
    int i = findProperty(readInt("Property ID to update: "));
    if (i < 0) { cout << "Record not found.\n"; return; }
    string address = readLine("New address: ");
    double rent = readDouble("New monthly rent: ");
    try {
        Property check(1, address, rent, 1);       // validate first
        properties[i]->setAddress(address);
        properties[i]->setRent(rent);
        cout << "Property updated.\n";
    } catch (invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void PropertySystem::updateTenant() {
    int i = findTenant(readInt("Tenant ID to update: "));
    if (i < 0) { cout << "Record not found.\n"; return; }
    string name = readLine("New name: ");
    string phone = readLine("New phone: ");
    try {
        Tenant check(1, name, phone);              // validate first
        tenants[i].setName(name);
        tenants[i].setPhone(phone);
        cout << "Tenant updated.\n";
    } catch (invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void PropertySystem::updateOwner() {
    int i = findOwner(readInt("Owner ID to update: "));
    if (i < 0) { cout << "Record not found.\n"; return; }
    string name = readLine("New name: ");
    string phone = readLine("New phone: ");
    try {
        Owner check(1, name, phone);               // validate first
        owners[i].setName(name);
        owners[i].setPhone(phone);
        cout << "Owner updated.\n";
    } catch (invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

void PropertySystem::updateRecord() {
    int t = chooseType();
    if (t == 1) updateProperty();
    else if (t == 2) updateTenant();
    else if (t == 3) updateOwner();
}

// ---------- 5. Delete ----------
void PropertySystem::deleteProperty() {
    int i = findProperty(readInt("Property ID to delete: "));
    if (i < 0) { cout << "Record not found.\n"; return; }
    if (properties[i]->isRented()) { cout << "Cannot delete: property is rented.\n"; return; }
    delete properties[i];
    for (int k = i; k < propCount - 1; k++) properties[k] = properties[k + 1];   // shift left
    propCount--;
    cout << "Property deleted.\n";
}

void PropertySystem::deleteTenant() {
    int i = findTenant(readInt("Tenant ID to delete: "));
    if (i < 0) { cout << "Record not found.\n"; return; }
    if (tenantInUse(tenants[i].getId())) { cout << "Cannot delete: tenant has an agreement.\n"; return; }
    for (int k = i; k < tenantCount - 1; k++) tenants[k] = tenants[k + 1];
    tenantCount--;
    cout << "Tenant deleted.\n";
}

void PropertySystem::deleteOwner() {
    int i = findOwner(readInt("Owner ID to delete: "));
    if (i < 0) { cout << "Record not found.\n"; return; }
    if (ownerInUse(owners[i].getId())) { cout << "Cannot delete: owner has properties.\n"; return; }
    for (int k = i; k < ownerCount - 1; k++) owners[k] = owners[k + 1];
    ownerCount--;
    cout << "Owner deleted.\n";
}

void PropertySystem::deleteRecord() {
    int t = chooseType();
    if (t == 1) deleteProperty();
    else if (t == 2) deleteTenant();
    else if (t == 3) deleteOwner();
}

// ---------- 6. Main transaction: create rental agreement ----------
void PropertySystem::createAgreement() {
    int pIndex = findProperty(readInt("Property ID: "));
    if (pIndex < 0) { cout << "Property not found.\n"; return; }
    if (properties[pIndex]->isRented()) { cout << "Property is already rented.\n"; return; }
    int tIndex = findTenant(readInt("Tenant ID: "));
    if (tIndex < 0) { cout << "Tenant not found.\n"; return; }
    int months = readInt("Number of months: ");
    try {
        RentalAgreement a(nextAgrId, properties[pIndex]->getId(), tenants[tIndex].getId(),
                          properties[pIndex]->getOwnerId(), months, properties[pIndex]->getRent());
        if (agrCount == agrCap) growAgreements();
        agreements[agrCount++] = a;
        properties[pIndex]->setRented(true);
        cout << "Agreement " << nextAgrId++ << " created. Total rent: " << a.getTotal() << "\n";
    } catch (invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

// ---------- 7. Report ----------
void PropertySystem::showReport() const {
    int rentedCount = 0;
    double availableRent = 0, activeMonthly = 0, contractTotal = 0;
    for (int i = 0; i < propCount; i++) {
        if (properties[i]->isRented()) rentedCount++;
        else availableRent += properties[i]->getRent();
    }
    for (int i = 0; i < agrCount; i++) {
        activeMonthly += agreements[i].getMonthlyRent();
        contractTotal += agreements[i].getTotal();
    }
    cout << "\n===== SUMMARY REPORT =====\n";
    cout << "Owners: " << ownerCount << "\nTenants: " << tenantCount << "\n";
    cout << "Properties: " << propCount << " (Rented: " << rentedCount
         << ", Available: " << propCount - rentedCount << ")\n";
    cout << "Agreements: " << agrCount << "\n";
    cout << "Monthly rent from agreements: " << activeMonthly << "\n";
    cout << "Total value of all agreements: " << contractTotal << "\n";
    cout << "Monthly rent of available properties: " << availableRent << "\n";
}

// ---------- 8. Save / Load ----------
void PropertySystem::saveData() {
    string* pl = new string[propCount];
    for (int i = 0; i < propCount; i++) pl[i] = properties[i]->toLine();
    string* tl = new string[tenantCount];
    for (int i = 0; i < tenantCount; i++) tl[i] = tenants[i].toLine();
    string* ol = new string[ownerCount];
    for (int i = 0; i < ownerCount; i++) ol[i] = owners[i].toLine();
    string* al = new string[agrCount];
    for (int i = 0; i < agrCount; i++) al[i] = agreements[i].toLine();

    bool ok = files.writeLines(PROPERTY_FILE, pl, propCount);
    ok = files.writeLines(TENANT_FILE, tl, tenantCount) && ok;
    ok = files.writeLines(OWNER_FILE, ol, ownerCount) && ok;
    ok = files.writeLines(AGREEMENT_FILE, al, agrCount) && ok;
    delete[] pl; delete[] tl; delete[] ol; delete[] al;

    if (ok) cout << "Data saved to the Data folder.\n";
    else cout << "Error: could not write files. Run the program from the project folder.\n";
}

void PropertySystem::loadData() {
    freeMemory();      // clear old data, start with fresh empty arrays
    initMemory();
    string parts[8];
    string* lines = nullptr;

    int n = files.readLines(OWNER_FILE, lines);
    for (int i = 0; i < n; i++) {
        try {
            files.split(lines[i], parts, 3);
            Owner o(stoi(parts[0]), parts[1], parts[2]);
            if (ownerCount == ownerCap) growOwners();
            owners[ownerCount++] = o;
            if (o.getId() >= nextOwnerId) nextOwnerId = o.getId() + 1;
        } catch (exception&) { cout << "Skipped bad owner line " << i + 1 << ".\n"; }
    }
    delete[] lines;

    n = files.readLines(TENANT_FILE, lines);
    for (int i = 0; i < n; i++) {
        try {
            files.split(lines[i], parts, 3);
            Tenant t(stoi(parts[0]), parts[1], parts[2]);
            if (tenantCount == tenantCap) growTenants();
            tenants[tenantCount++] = t;
            if (t.getId() >= nextTenantId) nextTenantId = t.getId() + 1;
        } catch (exception&) { cout << "Skipped bad tenant line " << i + 1 << ".\n"; }
    }
    delete[] lines;

    n = files.readLines(PROPERTY_FILE, lines);
    for (int i = 0; i < n; i++) {
        try {
            files.split(lines[i], parts, 7);
            Property* p = new ResidentialProperty(stoi(parts[1]), parts[2], stod(parts[3]),
                                                  stoi(parts[4]), stoi(parts[6]), parts[5] == "1");
            if (propCount == propCap) growProperties();
            properties[propCount++] = p;
            if (p->getId() >= nextPropId) nextPropId = p->getId() + 1;
        } catch (exception&) { cout << "Skipped bad property line " << i + 1 << ".\n"; }
    }
    delete[] lines;

    n = files.readLines(AGREEMENT_FILE, lines);
    for (int i = 0; i < n; i++) {
        try {
            files.split(lines[i], parts, 6);
            RentalAgreement a(stoi(parts[0]), stoi(parts[1]), stoi(parts[2]),
                              stoi(parts[3]), stoi(parts[4]), stod(parts[5]));
            if (agrCount == agrCap) growAgreements();
            agreements[agrCount++] = a;
            if (a.getId() >= nextAgrId) nextAgrId = a.getId() + 1;
        } catch (exception&) { cout << "Skipped bad agreement line " << i + 1 << ".\n"; }
    }
    delete[] lines;

    cout << "Loaded: " << ownerCount << " owners, " << tenantCount << " tenants, "
         << propCount << " properties, " << agrCount << " agreements.\n";
}

void PropertySystem::saveLoadMenu() {
    cout << "1. Save data  2. Load data\n";
    int c = readInt("Choose: ");
    if (c == 1) saveData();
    else if (c == 2) loadData();
    else cout << "Invalid choice.\n";
}

// ---------- menu loop ----------
void PropertySystem::showMenu() const {
    cout << "\n===== RENTAL PROPERTY MANAGEMENT =====\n"
         << "1. Add Record\n2. Display Records\n3. Search Record\n4. Update Record\n"
         << "5. Delete Record\n6. Create Rental Agreement\n7. Report\n"
         << "8. Save / Load Data\n9. Exit\n";
}

void PropertySystem::run() {
    loadData();                      // START: load existing records
    int choice = 0;
    while (choice != 9) {
        showMenu();
        choice = readInt("Enter choice: ");
        switch (choice) {
            case 1: addRecord(); break;
            case 2: displayRecords(); break;
            case 3: searchRecord(); break;
            case 4: updateRecord(); break;
            case 5: deleteRecord(); break;
            case 6: createAgreement(); break;
            case 7: showReport(); break;
            case 8: saveLoadMenu(); break;
            case 9: break;
            default: cout << "Invalid choice. Enter 1-9.\n";
        }
    }
    saveData();                      // EXIT: save updated records
    cout << "Goodbye!\n";
}
