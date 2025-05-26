//
// Created by adrian on 22/05/25.
//

#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <any>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "IService.h"

class ServiceLocator {
public:
    ServiceLocator() = default;
    ServiceLocator(const ServiceLocator&) = delete;
    ServiceLocator& operator=(const ServiceLocator&) = delete;

    template<typename T>
    void registerService(T* service) {
        static_assert(std::is_base_of_v<IService, T>, "T must derive from IService");

        const auto key = std::type_index(typeid(T));

        if (!service) throw std::runtime_error("Tentative d'enregistrement d'un service nul : " + typeName<T>());

        if (services.contains(key)) throw std::runtime_error("Service déjà enregistré : " + typeName<T>());

        services[key] = service;
        baseServices.push_back(service);
    }

    template<typename T>
    T& get() const {
         const auto key = std::type_index(typeid(T));
         const auto it = services.find(key);
         if (it == services.end()) throw std::runtime_error("Service introuvable : " + typeName<T>());

         T* ptr = std::any_cast<T*>(it->second);
         if (!ptr) throw std::runtime_error("Pointeur nul pour : " + typeName<T>());
         return *ptr;
     }

    void initAll() const {
        for (IService* service : baseServices) {
            if (!service) throw std::runtime_error("Pointeur IService nul dans baseServices");
            service->init(*this);
        }
    }

private:
    std::unordered_map<std::type_index, std::any> services;
    std::vector<IService*> baseServices;

    template<typename T>
    static std::string typeName() {
        return std::string(typeid(T).name());
    }
};

#endif //SERVICELOCATOR_H
