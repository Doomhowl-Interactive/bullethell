#include <stdexcept>

#include "bullet_common.hpp"
#include "bullet_service.hpp"

using namespace std;

template <typename T> std::shared_ptr<T> Service<T>::Initialize()
{
    if (instance == nullptr) {
        instance = std::make_shared<T>();
    } else {
        throw std::runtime_error("Service already initialized");
    }
    return instance;
}

template <typename T> Service<T>::Service(const string& name)
{
    this->name = name;
    string msg = name + " service initialized!";
    TraceLog(LOG_INFO, msg.c_str());
}

template <typename T> Service<T>::~Service()
{
    string msg = name + " service destroyed!";
    TraceLog(LOG_INFO, msg.c_str());
}

template <typename T> std::shared_ptr<T> Service<T>::Instance()
{
    return instance.lock();
}
