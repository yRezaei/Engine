#ifndef OA_EVENTBRIDGE_H
#define OA_EVENTBRIDGE_H

#include "eventhandler.h"

template <class> class EventHandlerQueue;

template <typename tEventType, class tHandler>
class EventBridge: public EventHandler<tEventType> {
private:
	friend class EventHandlerQueue<tEventType>;

	tHandler& mHandler;		

	EventBridge(tHandler& handler);

	void handle(const tEventType& type);

public:
	bool operator == (const tHandler& handler) const;
};

//Implementation
template <typename T, class U>
EventBridge<T, U>::EventBridge(U& handler): 
	mHandler(handler) 
{
}

template <typename T, class U>
void EventBridge<T, U>::handle(const T& object) {
	mHandler.handle(object);
}

template <typename T, class U>
bool EventBridge<T, U>::operator == (const U& handler) const {
	return ((&mHandler) == (&handler));
}

#endif