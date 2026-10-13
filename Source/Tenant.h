#ifndef TENANT_H
#define TENANT_H
#include <string>

class Tenant {
private:
    int id;
    std::string name;
    std::string phone;
public:
    Tenant();
    Tenant(int id, std::string name, std::string phone);
    ~Tenant();

    int getId() const;
    std::string getName() const;
    std::string getPhone() const;
    void setName(std::string name);
    void setPhone(std::string phone);

    void display() const;
    std::string toLine() const;
};
#endif
