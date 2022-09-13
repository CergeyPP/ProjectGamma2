#pragma once
#include <list>
#include <algorithm>

namespace extend {
	template<class ...TParams>
	class AbstractEventHandler {
		using MyType = AbstractEventHandler<TParams...>;
	public:
		virtual void call(TParams... params) = 0;

		bool operator==(const MyType& other) {
			return isEquals(other);
		}
		bool operator!=(const MyType& other) {
			return !(*this == other);
		}

		virtual ~AbstractEventHandler() {}

	protected:
		AbstractEventHandler() {}


		virtual bool isEquals(const MyType& other) = 0;
	};

	template<class TObject, class... TParams>
	class EventHandler : public AbstractEventHandler<TParams...> {
		using TMethod = void(TObject::*)(TParams...);
	public:

		EventHandler(TObject& object, TMethod method) : AbstractEventHandler<TParams...>(), m_object(object), m_method(method) {

		}
		~EventHandler() override {}
		
		void call(TParams... params) override final {
			(m_object.*m_method)(params...);
		}

	private:

		TObject& m_object;
		TMethod m_method;

		bool isEquals(const AbstractEventHandler<TParams...>& other) override {
			const EventHandler<TObject, TParams...>* _other = dynamic_cast<const EventHandler<TObject, TParams...>*>(&other);
			return (_other != nullptr && &m_object == &(_other->m_object) && m_method == (_other->m_method));

		}
	};

	template<class ...TParams>
	class TEvent {
	public:

		~TEvent() {
			for (auto elem : m_handlers) {
				delete elem;
			}
			m_handlers.clear();
		}

		void operator()(TParams... params) {
			for (auto elem : m_handlers) {
				elem->call(params...);
			}
		}

		bool operator+=(AbstractEventHandler<TParams...>* handler) {
			auto it = std::find_if(m_handlers.cbegin(), m_handlers.cend(), [&handler](const AbstractEventHandler<TParams...>* oneHandler)
				{
					return (*oneHandler == *handler);
				});
			if (it == m_handlers.end()) {
				m_handlers.push_back(handler);
				return true;
			}
			else {
				delete handler;
				return false;
			}
		}

		bool operator-=(AbstractEventHandler<TParams...>* handler) {
			auto it = std::find_if(m_handlers.cbegin(), m_handlers.cend(), [&handler](const AbstractEventHandler<TParams...>* oneHandler)
				{
					return (*oneHandler == *handler);
				});
			if (it != m_handlers.end()) {
				delete (*it);
				m_handlers.erase(it);
				delete handler;
				return true;
			}
			else {
				delete handler;
				return false;
			}
		}

	private:
		std::list<AbstractEventHandler<TParams...>*> m_handlers;


	};

	template<class TObject, class ...TParams>
	AbstractEventHandler<TParams...>* createMethodEventHandler(TObject& object, void(TObject::* method)(TParams...))
	{
		return new EventHandler<TObject, TParams...>(object, method);
	}
}

#define METHOD_HANDLER( Object, Method ) extend::createMethodEventHandler( Object, &Method )
#define MY_METHOD_HANDLER( Method ) METHOD_HANDLER( *this, Method )

