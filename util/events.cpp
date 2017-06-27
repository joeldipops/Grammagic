#include "events.h"

#include <SDL2/SDL.h>

using namespace Core;

/**
 * Populates the passed in event if an event is available.
 * Returns true if there was an event, otherwise false.
 */
bool EventManager::pollEvent(Event* event, EventData* data)
{
    // Ensure the queue is full of events.
    queueEvents();
    if (event == nullptr)
        return false;

    if (_queue.size() <= 0)
        return false;

    *event = _queue.front();
    _queue.pop();

    // Take care of callbacks immediately.
    if (event->Callback)
        event->Callback(event, *data);

    return true;
}

/**
 * Add an event to the end of the queue.
 * @param event The event.
 */
void EventManager::addEvent(const Event& event)
{
    _queue.push(event);
}

/**
 * Ensures all SDL_Events are in the queue.
 */
void EventManager::queueEvents(void)
{
    SDL_Event sdlEvent;
    while(SDL_PollEvent(&sdlEvent))
    {
        Event event;
        event.InnerEvent = new SDL_Event(sdlEvent);
        _queue.push(event);
    }
}
