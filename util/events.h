#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include <queue>
#include <SDL2/SDL.h>

template<typename Context,  typename T>
using Handler = T& (*) (Context*, T&);

namespace Core
{
    struct EventData {};
    struct Event
    {
        ~Event(void)
        {
            if (InnerEvent != nullptr)
                //delete InnerEvent;
            InnerEvent = nullptr;
        };
        SDL_Event* InnerEvent = nullptr;
        Handler<Event, EventData> Callback = nullptr;
    };

    class EventManager final
    {
        public:
            bool pollEvent(Event*, EventData* = nullptr);
            void addEvent(const Event&);
        private:
            void queueEvents(void);
            std::queue<Event> _queue;
    };
}

extern Core::EventManager eventManager;

#endif
