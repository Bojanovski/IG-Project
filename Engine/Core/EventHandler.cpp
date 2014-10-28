/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/EventHandler.h>
#include "SDL.h"

using namespace std;

namespace engine
{
    vector<EventListener*> EventHandler::listenerList;
    vector<Updateable*> EventHandler::updateableList;
    bool EventHandler::quit = false;
    float EventHandler::timeStep = 1.0f / 60.0f;
    float EventHandler::accumulator = 0.0f;

    void EventHandler::ProcessPolledEvents()
    {
        SDL_Event test_event;
        while(SDL_PollEvent(&test_event))
        {
            if(test_event.type == SDL_QUIT || (test_event.type == SDL_KEYDOWN && test_event.key.keysym.sym == SDLK_ESCAPE))
            {
                quit = true;
                ClearEventListenerList();
                ClearUpdateableList();
                return;
            }

            for(EventListener *listener : listenerList)
                if(listener->active)
                    listener->HandleEvent(test_event);
        }
    }

    void EventHandler::AddEventListener(EventListener *listener)
    {
        listenerList.push_back(listener);
    }

    void EventHandler::RemoveEventListener(const EventListener *listener)
    {
        //TODO
    }

    void EventHandler::ClearEventListenerList()
    {
        listenerList.clear();
    }

    bool EventHandler::Quit()
    {
        return quit;
    }

    void EventHandler::Update()
    {
        static float lastTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        const float currentTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        accumulator += currentTime - lastTime;
        lastTime = currentTime;

        for(; accumulator > timeStep; accumulator -= timeStep)
            for(Updateable *updateable : updateableList)
                if(updateable->active)
                    updateable->Update(timeStep);
    }

    void EventHandler::AddUpdateable(Updateable *updateable)
    {
        updateableList.push_back(updateable);
    }

    void EventHandler::RemoveUpdateable(const Updateable *updateable)
    {
        //TODO
    }

    void EventHandler::ClearUpdateableList()
    {
        updateableList.clear();
    }
}
