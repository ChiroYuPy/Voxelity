//
// Created by adrian on 22/05/25.
//

#ifndef ISERVICE_H
#define ISERVICE_H

class ServiceLocator;

class IService {
public:
    virtual ~IService() = default;
    virtual void init(const ServiceLocator& locator) = 0;
};

#endif //ISERVICE_H
