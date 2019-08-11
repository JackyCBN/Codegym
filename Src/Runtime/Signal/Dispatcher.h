#pragma once


#include "Base.h"
#include "signal.h"
#include "Core/Family.h"
#include "Core/TypeTraits.h"

namespace codegym::runtime {

/**
 * @brief Basic dispatcher implementation.
 *
 * A dispatcher can be used either to trigger an immediate event or to enqueue
 * events to be published all together once per tick.<br/>
 * Listeners are provided in the form of member functions. For each event of
 * type `Event`, listeners are such that they can be invoked with an argument of
 * type `const Event &`, no matter what the return type is.
 *
 * The type of the instances is `Class *` (a naked pointer). It means that users
 * must guarantee that the lifetimes of the instances overcome the one of the
 * dispatcher itself to avoid crashes.
 */
class dispatcher {
    using event_family = family<struct internal_dispatcher_event_family>;

    template<typename Class, typename Event>
    using instance_type = typename signal<void(const Event &)>::template instance_type<Class>;

    struct base_wrapper {
        virtual ~base_wrapper() = default;
        virtual void publish() = 0;
    };

    template<typename Event>
    struct signal_wrapper: base_wrapper {
        using signal_type = signal<void(const Event &)>;
        using sink_type = typename signal_type::sink_type;

        void publish() override {
			int cur = current;
			++current;
            for(const auto &event: events[cur]) {
                signal.publish(event);
            }

            events[cur].clear();
            current %= extent<decltype(events)>::value;
        }

        sink_type sink() CG_NOEXCEPT {
            return sink{signal};
        }

        template<typename... Args>
        void trigger(Args &&... args) {
            signal.publish({ forward<Args>(args)... });
        }

        template<typename... Args>
        void enqueue(Args &&... args) {
            events[current].emplace_back(forward<Args>(args)...);
        }

    private:
        signal_type signal{};
        vector<Event> events[2];
        int current{};
    };

    struct wrapper_data {
        unique_ptr<base_wrapper> wrapper;
        GC_ID_TYPE runtime_type;
    };

    template<typename Event>
    static auto type() CG_NOEXCEPT {
        if constexpr(is_named_type_v<Event>) {
            return named_type_traits<Event>::value;
        } else {
            return event_family::type<Event>;
        }
    }

    template<typename Event>
    signal_wrapper<Event> & assure() {
        const auto wtype = type<Event>();
        wrapper_data *wdata = nullptr;

        if constexpr(is_named_type_v<Event>) {
            const auto it = find_if(wrappers.begin(), wrappers.end(), [wtype](const auto &candidate) {
                return candidate.wrapper && candidate.runtime_type == wtype;
            });

            wdata = (it == wrappers.cend() ? &wrappers.emplace_back() : &(*it));
        } else {
            if(!(wtype < wrappers.size())) {
                wrappers.resize(wtype+1);
            }

            wdata = &wrappers[wtype];

            if(wdata->wrapper && wdata->runtime_type != wtype) {
                wrappers.emplace_back();
                swap(wrappers[wtype], wrappers.back());
                wdata = &wrappers[wtype];
            }
        }

        if(!wdata->wrapper) {
            wdata->wrapper = make_unique<signal_wrapper<Event>>();
            wdata->runtime_type = wtype;
        }

        return static_cast<signal_wrapper<Event> &>(*wdata->wrapper);
    }

public:
    /*! @brief Type of sink for the given event. */
    template<typename Event>
    using sink_type = typename signal_wrapper<Event>::sink_type;

    /**
     * @brief Returns a sink object for the given event.
     *
     * A sink is an opaque object used to connect listeners to events.
     *
     * The function type for a listener is:
     * @code{.cpp}
     * void(const Event &);
     * @endcode
     *
     * The order of invocation of the listeners isn't guaranteed.
     *
     * @sa sink
     *
     * @tparam Event Type of event of which to get the sink.
     * @return A temporary sink object.
     */
    template<typename Event>
    sink_type<Event> sink() CG_NOEXCEPT {
        return assure<Event>().sink();
    }

    /**
     * @brief Triggers an immediate event of the given type.
     *
     * All the listeners registered for the given type are immediately notified.
     * The event is discarded after the execution.
     *
     * @tparam Event Type of event to trigger.
     * @tparam Args Types of arguments to use to construct the event.
     * @param args Arguments to use to construct the event.
     */
    template<typename Event, typename... Args>
    void trigger(Args &&... args) {
        assure<Event>().trigger(forward<Args>(args)...);
    }

    /**
     * @brief Triggers an immediate event of the given type.
     *
     * All the listeners registered for the given type are immediately notified.
     * The event is discarded after the execution.
     *
     * @tparam Event Type of event to trigger.
     * @param event An instance of the given type of event.
     */
    template<typename Event>
    void trigger(Event &&event) {
        assure<decay_t<Event>>().trigger(forward<Event>(event));
    }

    /**
     * @brief Enqueues an event of the given type.
     *
     * An event of the given type is queued. No listener is invoked. Use the
     * `update` member function to notify listeners when ready.
     *
     * @tparam Event Type of event to enqueue.
     * @tparam Args Types of arguments to use to construct the event.
     * @param args Arguments to use to construct the event.
     */
    template<typename Event, typename... Args>
    void enqueue(Args &&... args) {
        assure<Event>().enqueue(forward<Args>(args)...);
    }

    /**
     * @brief Enqueues an event of the given type.
     *
     * An event of the given type is queued. No listener is invoked. Use the
     * `update` member function to notify listeners when ready.
     *
     * @tparam Event Type of event to enqueue.
     * @param event An instance of the given type of event.
     */
    template<typename Event>
    void enqueue(Event &&event) {
        assure<decay_t<Event>>().enqueue(forward<Event>(event));
    }

    /**
     * @brief Delivers all the pending events of the given type.
     *
     * This method is blocking and it doesn't return until all the events are
     * delivered to the registered listeners. It's responsibility of the users
     * to reduce at a minimum the time spent in the bodies of the listeners.
     *
     * @tparam Event Type of events to send.
     */
    template<typename Event>
    void update() {
        assure<Event>().publish();
    }

    /**
     * @brief Delivers all the pending events.
     *
     * This method is blocking and it doesn't return until all the events are
     * delivered to the registered listeners. It's responsibility of the users
     * to reduce at a minimum the time spent in the bodies of the listeners.
     */
    void update() const {
        for(auto pos = wrappers.size(); pos; --pos) {
            auto &wdata = wrappers[pos-1];

            if(wdata.wrapper) {
                wdata.wrapper->publish();
            }
        }
    }

private:
    vector<wrapper_data> wrappers;
};


}
