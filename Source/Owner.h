#ifndef OWNER_H
#define OWNER_H
#include <string>

class Owner {
private:
    int id;
    std::string name;
    std::string phone;
public:
    Owner();
    Owner(int id, std::string name, std::string phone);
    ~Owner();

    int getId() const;
    std::string getName() const;
    std::string getPhone() const;
    void setName(std::string name);
    void setPhone(std::string phone);

    void display() const;
    std::string toLine() const;
};
#endif
