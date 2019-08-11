#pragma once

#include "Base.h"
namespace codegym::runtime
{
	template<typename Element>
	class EventList
	{
	public:
		using element_type = pair<bool, Element>;
		using call_type = function<void(element_type&&)>;
		using container_type = list<element_type>;
		using connection_type = typename container_type::iterator;

		bool empty() const CG_NOEXCEPT {
			auto pred = [](auto&& element) { return element.first; };

			return all_of(m_list.cbegin(), m_list.cend(), pred);
		}

		void clear() CG_NOEXCEPT {
			if (publishing) {
				auto func = [](auto&& element) { element.first = true; };
				for_each(m_list.begin(), m_list.end(), func);
			}
			else {
				m_list.clear();
			}
		}

		connection_type add(Element listener) {
			return m_list.emplace(m_list.cend(), false, move(listener));
		}

		void erase(connection_type conn) CG_NOEXCEPT {
			conn->first = true;

			if (!publishing) {
				auto pred = [](auto&& element) { return element.first; };
				m_list.remove_if(pred);
			}
		}

		void publishOnce(call_type func) {
			container_type swap_list;
			m_list.swap(swap_list);

			publishing = true;

			for_each(m_list.rbegin(), m_list.rend(), func);

			publishing = false;
		}

		void publish(call_type func = nullptr, bool once=false) {
			func = func || [](Element&& element) {
				return element.first ? void() : element.second();
			};

			if(once)
			{
				publishOnce(func);
			}
			else
			{
				on(func);
			}
		}

	private:
		void on(call_type func)
		{
			publishing = true;

			for_each(m_list.rbegin(), m_list.rend(), func);

			publishing = false;

			m_list.remove_if([](auto&& element) { return element.first; });
		}
		
		bool publishing{ false };
		container_type m_list{};
	};
}