#pragma once
#include <functional>
#include <vector>
#include <algorithm>
#include <string>

template <typename Listener, typename... Args> class FunctionMap {
   public:
    std::string subscribe(std::string name, const Listener& listener)
    {
        listeners[name] = listener;
        return name;
    }

    template <typename... PublishArgs>
    std::string subscribeAndRun(std::string name, const Listener& listener, PublishArgs&&... args)
    {
        // If the listener is not already subscribed, subscribe it and run it
        if (listeners.find(name) == listeners.end()) {
            listeners[name] = listener;
            listener(std::forward<PublishArgs>(args)...);
        }
        return name;
    }

    void unsubscribe(std::string name)
    {
        listeners.erase(name);
    }

    template <typename... PublishArgs> size_t dispatch(PublishArgs&&... args)
    {
        for (const auto& [name, listener] : listeners) {
            listener(std::forward<PublishArgs>(args)...);
        }
        return listeners.size();
    }

   private:
    std::unordered_map<std::string, Listener> listeners;
};
