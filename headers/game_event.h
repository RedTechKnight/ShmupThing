#ifndef GAMEEVENT_H_INCLUDED
#define GAMEEVENT_H_INCLUDED
#include <any>
#include <list>
#include <queue>
const unsigned int FIRE_EVENT = 1;
const unsigned int EXPLOSION_SOUND_EVENT = 2;
const unsigned int EXPLOSION_EFFECT_EVENT = 3;
const unsigned int HIT_EVENT = 4;

template <typename T> class GameEventHandler {
private:
  std::list<T> mEvents;

public:
  GameEventHandler() { mEvents.clear(); }
  ~GameEventHandler() { mEvents.clear(); }

  void push_events(std::list<T> &eventList) {
    std::insert_iterator<std::list<T>> inserter(mEvents, mEvents.end());
    std::move(eventList.begin(), eventList.end(), inserter);
    eventList.clear();
  }

  T get_last_event() {
    T event = *mEvents.begin();
    mEvents.pop_front();
    return event;
  }

  unsigned int num_events() { return mEvents.size(); }
};
#endif // GAMEEVENT_H_INCLUDED
