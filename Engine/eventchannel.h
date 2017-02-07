#ifndef OA_EVENTCHANNEL_H
#define OA_EVENTCHANNEL_H

#include "eventhandlerqueue.h"

class EventChannel {
public:
	template <typename tEvent, class tHandler>
	void add(tHandler& handler) {
		EventHandlerQueue<tEvent>::instance().add<tHandler>(handler);
	}

	template <typename tEvent, class tHandler>
	void remove(const tHandler& handler) {
		EventHandlerQueue<tEvent>::instance().remove<tHandler>(handler);
	}

	template <typename tEvent>
	void broadcast(const tEvent& object) {
		EventHandlerQueue<tEvent>::instance().broadcast(object);
	}
};

#endif