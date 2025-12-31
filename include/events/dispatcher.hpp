// dispatcher.hpp, created by Andrew Gossen.

#include <vector>
#include <functional>
#include <utility>
#include "events/events.hpp"

namespace events{

class Dispatcher{

public:

    // A handler is a calleable that accepts an event
    // 
    using Handler = std::function<void(const Event&)>;

    void subscribe(Handler h){ // Register a new handler that will receive every dispatched event 
        m_handlers.push_back(std::move(h)); // Avoid un-needed copy 
    }
    void push(Event e){ // Add a new event to the schedule, processed immediately. 
        m_queue.push_back(std::move(e)); // Yet again, use the r-value overload to avoid copying 
    }

    bool empty() const {  
        return m_queue.empty();
    }

    void dispatchSingle(){ 

        // Dispatch a single event (One at front of queue) to all handler functions  

        Event e=std::move(m_queue.front()); // Get the first event in the queue 
        m_queue.erase(m_queue.begin());

        for (auto& handler : m_handlers){ // Dispatch this event to all handler functions 
            handler(e);
        }

    }

    void dispatchAll(){ 

        // Dispatch all events in the queue 

        while (!empty()){
            dispatchSingle();
        }

    }


private:
    
    // FIFO queue for pending events 
    std::vector<Event> m_queue;
    // Registered event handlers 
    std::vector<Handler> m_handlers;

};


}