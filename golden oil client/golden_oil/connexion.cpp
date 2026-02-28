#include "connexion.h"

#include <utility>

class connexionData : public QSharedData
{
public:
};

connexion::connexion()
    : data(new connexionData)
{}

connexion::connexion(const connexion &rhs)
    : data{rhs.data}
{}

connexion::connexion(connexion &&rhs)
    : data{std::move(rhs.data)}
{}

connexion &connexion::operator=(const connexion &rhs)
{
    if (this != &rhs)
        data = rhs.data;
    return *this;
}

connexion &connexion::operator=(connexion &&rhs)
{
    if (this != &rhs)
        data = std::move(rhs.data);
    return *this;
}

connexion::~connexion() {}
