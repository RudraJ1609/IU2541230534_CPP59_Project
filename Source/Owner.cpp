#include "Owner.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Owner::Owner() { id = 0; name = ""; phone = ""; }

Owner::Owner(int id, string name, string phone) {
    if (id <= 0) throw invalid_argument("Owner ID must be positive.");
    this->id = id;
    setName(name);
    setPhone(phone);
}

Owner::~Owner() {}

int Owner::getId() const { return id; }
string Owner::getName() const { return name; }
string Owner::getPhone() const { return phone; }

void Owner::setName(string name) {
    if (name.empty() || name.find('|') != string::npos)
        throw invalid_argument("Name cannot be empty or contain '|'.");
    this->name = name;
}

void Owner::setPhone(string phone) {
    if (phone.size() != 10) throw invalid_argument("Phone must have exactly 10 digits.");
    for (size_t i = 0; i < phone.size(); i++)
        if (phone[i] < '0' || phone[i] > '9')
            throw invalid_argument("Phone must contain digits only.");
    this->phone = phone;
}

void Owner::display() const {
    cout << "Owner " << id << " | " << name << " | Phone: " << phone << "\n";
}

string Owner::toLine() const {
    return to_string(id) + "|" + name + "|" + phone;
}
