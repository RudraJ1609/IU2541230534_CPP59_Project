#include "Tenant.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Tenant::Tenant() { id = 0; name = ""; phone = ""; }

Tenant::Tenant(int id, string name, string phone) {
    if (id <= 0) throw invalid_argument("Tenant ID must be positive.");
    this->id = id;
    setName(name);
    setPhone(phone);
}

Tenant::~Tenant() {}

int Tenant::getId() const { return id; }
string Tenant::getName() const { return name; }
string Tenant::getPhone() const { return phone; }

void Tenant::setName(string name) {
    if (name.empty() || name.find('|') != string::npos)
        throw invalid_argument("Name cannot be empty or contain '|'.");
    this->name = name;
}

void Tenant::setPhone(string phone) {
    if (phone.size() != 10) throw invalid_argument("Phone must have exactly 10 digits.");
    for (size_t i = 0; i < phone.size(); i++)
        if (phone[i] < '0' || phone[i] > '9')
            throw invalid_argument("Phone must contain digits only.");
    this->phone = phone;
}

void Tenant::display() const {
    cout << "Tenant " << id << " | " << name << " | Phone: " << phone << "\n";
}

string Tenant::toLine() const {
    return to_string(id) + "|" + name + "|" + phone;
}
