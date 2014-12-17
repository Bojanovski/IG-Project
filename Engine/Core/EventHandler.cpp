/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/EventHandler.h>
#include "SDL.h"
#include "Leap.h"

using namespace std;

namespace engine
{
    vector<EventListener*> EventHandler::listenerList;
    vector<Updateable*> EventHandler::updateableList;
    bool EventHandler::quit = false;
    bool EventHandler::isCursorFree = true;
    float EventHandler::timeStep = 1.0f / 60.0f;
	float EventHandler::accumulator = 0.0f;

	Leap::Controller EventHandler::leap;

	struct VirtualKey {
		VirtualKey(int key) :
		key(key), state(0) {}

		void set(bool newState) {
			if(newState != state) {
				state = newState;
				SDL_KeyboardEvent ke;
				ke.keysym.sym = key;
				ke.repeat = 0;
				ke.state = state ? SDL_PRESSED : SDL_RELEASED;
				ke.type = state ? SDL_KEYDOWN : SDL_KEYUP;
				SDL_Event e;
				e.key = ke;
				for(EventListener *listener : EventHandler::listenerList)
				if(listener->active)
					listener->HandleEvent(e);
			}
		}

		bool state;
		int key;
	};

    void EventHandler::ProcessPolledEvents()
    {
        SDL_Event test_event;
        while(SDL_PollEvent(&test_event))
        {
            if(test_event.type == SDL_QUIT || (test_event.type == SDL_KEYDOWN && test_event.key.keysym.sym == SDLK_ESCAPE))
                quit = true;
            else if(test_event.type == SDL_KEYDOWN && test_event.key.keysym.sym == SDLK_LSHIFT)
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                SDL_ShowCursor(1);
                isCursorFree = false;
            }
            else if(test_event.type == SDL_KEYUP && test_event.key.keysym.sym == SDLK_LSHIFT)
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                SDL_ShowCursor(0);
                isCursorFree = true;
            }
            else if(test_event.type == SDL_MOUSEMOTION && isCursorFree == false)
                continue;

            for(EventListener *listener : listenerList)
                if(listener->active)
                    listener->HandleEvent(test_event);
        }

		static VirtualKey l(SDLK_j), r(SDLK_l), f(SDLK_i), b(SDLK_k);

		Leap::HandList hands = leap.frame().hands();
		if(!hands.isEmpty()) {
			Leap::Hand hand = hands[0];
			Leap::Vector pn = hand.palmNormal();
			if(pn.y > -0.7) {
				r.set(pn.x > 0.6);
				f.set(pn.z < -0.2);
				l.set(pn.x < -0.6);
				b.set(pn.z > 0.3);
			}
			else {
				r.set(0);
				f.set(0);
				l.set(0);
				b.set(0);
			}
		}
		else {
			r.set(0);
			f.set(0);
			l.set(0);
			b.set(0);
		}
    }

    void EventHandler::AddEventListener(EventListener *listener)
    {
        listenerList.push_back(listener);
    }

    void EventHandler::RemoveEventListener(const EventListener *listener)
    {
        //TODO_JURE finish
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
        //TODO_JURE finish
    }
}
