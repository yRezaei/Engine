#ifndef OA_EVENTHANDLERQUEUE_H
#define OA_EVENTHANDLERQUEUE_H

#include "eventbridge.h"
#include <list>
#include <boost/shared_ptr.hpp> //optional, for use with smart pointers (default)
#include <boost/pointer_cast.hpp> //optional, for use with smart pointers (default)

class EventChannel; //forward definition

template <typename tEvent>
class EventHandlerQueue {
private: //the entire class is private, so no object may use it
	friend class EventChannel; //except for the EventChannel

	typedef EventHandler<tEvent> EventHandlerType;
		
	typedef boost::shared_ptr<EventHandlerType> EventHandlerPtr; //optional, if you want to use raw pointers uncomment below
	//typedef EventHandlerType* EventHandlerPtr; //raw pointer typedef
		
	typedef std::list<EventHandlerPtr> HandlerList;
	typedef typename HandlerList::iterator HandlerIterator;
	typedef typename HandlerList::const_iterator ConstHandlerIterator;

	HandlerList mHandlerList;
	
	//this destructor is only needed for use with raw pointers
	/*
	~EventHandlerQueue() {
		for (HandlerIterator it = mHandlerList.begin(); it != mHandlerList.end(); ++it)
			delete (*it);
	}
	*/

	static EventHandlerQueue& instance() {
		static EventHandlerQueue anInstance;
		return anInstance;
	}

	template <class tHandler>
	void add(tHandler& handler) {
		mHandlerList.push_back(
			EventHandlerPtr(new EventBridge<tEvent, tHandler>(handler))
			//new EventBridge<tEvent, tHandler>(handler) //for use with raw pointers
		);
	}

	void broadcast(const tEvent& object) {
		ConstHandlerIterator next;
		for (ConstHandlerIterator it = mHandlerList.begin(); it != mHandlerList.end(); ) {
			//the handle function might invalidate the iterator, so make a copy and advance immediately
			next = it++;
			(*next)->handle(object);
		}
	}

	//comparison predicate class (for removing handlers)
	template <class tHandler>
	class PointsToSame {
	public:
		typedef EventBridge<tEvent, tHandler> BridgeType;

		const tHandler& mHandler;

		PointsToSame(const tHandler& h): mHandler(h) {}

		bool operator()(EventHandlerPtr ptr) {				
			return ((*boost::static_pointer_cast<BridgeType>(ptr)) == mHandler);
			//return ((*static_cast<BridgeType*>(ptr)) == mHandler); //for use with raw pointers
		}
	};

	template <class tHandler>
	void remove(const tHandler& handler) {
		PointsToSame<tHandler> pts(handler);
		mHandlerList.remove_if(pts);
	}
};

#endif