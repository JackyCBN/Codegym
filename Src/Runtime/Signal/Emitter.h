#pragma once


#include "Base.h"
#include "Core/TypeTraits.h"
#include "Core/Family.h"


namespace codegym::runtime {


/**
 * @brief General purpose event Emitter.
 *
 * The Emitter class template follows the CRTP idiom. To create a custom Emitter
 * type, derived classes must inherit directly from the base class as:
 *
 * @code{.cpp}
 * struct my_emitter: Emitter<my_emitter> {
 *     // ...
 * }
 * @endcode
 *
 * Handlers for the type of events are created internally on the fly. It's not
 * required to specify in advance the full list of accepted types.<br/>
 * Moreover, whenever an event is published, an Emitter provides the listeners
 * with a reference to itself along with a const reference to the event.
 * Therefore listeners have an handy way to work with it without incurring in
 * the need of capturing a reference to the Emitter.
 *
 * @tparam Derived Actual type of Emitter that extends the class template.
 */
template<typename Derived>
class Emitter {
    using handler_family = family<struct internal_emitter_handler_family>;

    struct base_handler {
        virtual ~base_handler() = default;
        virtual bool empty() const CG_NOEXCEPT = 0;
        virtual void clear() CG_NOEXCEPT = 0;
    };

    template<typename Event>
    struct event_handler: base_handler {
        using listener_type = function<void(const Event &, Derived &)>;
        using element_type = pair<bool, listener_type>;
        using container_type = list<element_type>;
        using connection_type = typename container_type::iterator;

        bool empty() const CG_NOEXCEPT override {
            auto pred = [](auto &&element) { return element.first; };

            return all_of(once_list.cbegin(), once_list.cend(), pred) &&
                    all_of(on_list.cbegin(), on_list.cend(), pred);
        }

        void clear() CG_NOEXCEPT override {
            if(publishing) {
                auto func = [](auto &&element) { element.first = true; };
                for_each(once_list.begin(), once_list.end(), func);
                for_each(on_list.begin(), on_list.end(), func);
            } else {
                once_list.clear();
                on_list.clear();
            }
        }

        connection_type once(listener_type listener) {
            return once_list.emplace(once_list.cend(), false, move(listener));
        }

        connection_type on(listener_type listener) {
            return on_list.emplace(on_list.cend(), false, move(listener));
        }

        void erase(connection_type conn) CG_NOEXCEPT {
            conn->first = true;

            if(!publishing) {
                auto pred = [](auto &&element) { return element.first; };
                once_list.remove_if(pred);
                on_list.remove_if(pred);
            }
        }

        void publish(const Event &event, Derived &ref) {
            container_type swap_list;
            once_list.swap(swap_list);

            auto func = [&event, &ref](auto &&element) {
                return element.first ? void() : element.second(event, ref);
            };

            publishing = true;

            for_each(on_list.rbegin(), on_list.rend(), func);
            for_each(swap_list.rbegin(), swap_list.rend(), func);

            publishing = false;

            on_list.remove_if([](auto &&element) { return element.first; });
        }

    private:
        bool publishing{false};
        container_type once_list{};
        container_type on_list{};
    };

    struct handler_data {
        unique_ptr<base_handler> handler;
        GC_ID_TYPE runtime_type;
    };

    template<typename Event>
    static auto type() CG_NOEXCEPT {
        if constexpr(is_named_type_v<Event>) {
            return named_type_traits<Event>::value;
        } else {
            return handler_family::type<Event>;
        }
    }

    template<typename Event>
    event_handler<Event> * assure() const CG_NOEXCEPT {
        const auto htype = type<Event>();
        handler_data *hdata = nullptr;

        if constexpr(is_named_type_v<Event>) {
            const auto it = find_if(handlers.begin(), handlers.end(), [htype](const auto &candidate) {
                return candidate.handler && candidate.runtime_type == htype;
            });

            hdata = (it == handlers.cend() ? &handlers.emplace_back() : &(*it));
        } else {
            if(!(htype < handlers.size())) {
                handlers.resize(htype+1);
            }

            hdata = &handlers[htype];

            if(hdata->handler && hdata->runtime_type != htype) {
                handlers.emplace_back();
                swap(handlers[htype], handlers.back());
                hdata = &handlers[htype];
            }
        }

        if(!hdata->handler) {
            hdata->handler = make_unique<event_handler<Event>>();
            hdata->runtime_type = htype;
        }

        return static_cast<event_handler<Event> *>(hdata->handler.get());
    }

public:
    /** @brief Type of listeners accepted for the given event. */
    template<typename Event>
    using listener = typename event_handler<Event>::listener_type;

    /**
     * @brief Generic connection type for events.
     *
     * Type of the connection object returned by the event Emitter whenever a
     * listener for the given type is registered.<br/>
     * It can be used to break connections still in use.
     *
     * @tparam Event Type of event for which the connection is created.
     */
    template<typename Event>
    struct connection: private event_handler<Event>::connection_type {
        /** @brief Event emitters are friend classes of connections. */
        friend class Emitter;

        /*! @brief Default constructor. */
        connection() CG_NOEXCEPT = default;

        /**
         * @brief Creates a connection that wraps its underlying instance.
         * @param conn A connection object to wrap.
         */
        connection(typename event_handler<Event>::connection_type conn)
            : event_handler<Event>::connection_type{move(conn)}
        {}
    };

    /*! @brief Default constructor. */
    Emitter() CG_NOEXCEPT = default;

    /*! @brief Default destructor. */
    virtual ~Emitter() CG_NOEXCEPT {
        static_assert(is_base_of_v<Emitter<Derived>, Derived>);
    }

    /*! @brief Default move constructor. */
    Emitter(Emitter &&) = default;

    /*! @brief Default move assignment operator. @return This Emitter. */
    Emitter & operator=(Emitter &&) = default;

    /**
     * @brief Emits the given event.
     *
     * All the listeners registered for the specific event type are invoked with
     * the given event. The event type must either have a proper constructor for
     * the arguments provided or be an aggregate type.
     *
     * @tparam Event Type of event to publish.
     * @tparam Args Types of arguments to use to construct the event.
     * @param args Parameters to use to initialize the event.
     */
    template<typename Event, typename... Args>
    void publish(Args &&... args) {
        assure<Event>()->publish({ forward<Args>(args)... }, *static_cast<Derived *>(this));
    }

    /**
     * @brief Registers a long-lived listener with the event Emitter.
     *
     * This method can be used to register a listener designed to be invoked
     * more than once for the given event type.<br/>
     * The connection returned by the method can be freely discarded. It's meant
     * to be used later to disconnect the listener if required.
     *
     * The listener is as a callable object that can be moved and the type of
     * which is `void(const Event &, Derived &)`.
     *
     * @note
     * Whenever an event is emitted, the Emitter provides the listener with a
     * reference to the derived class. Listeners don't have to capture those
     * instances for later uses.
     *
     * @tparam Event Type of event to which to connect the listener.
     * @param instance The listener to register.
     * @return Connection object that can be used to disconnect the listener.
     */
    template<typename Event>
    connection<Event> on(listener<Event> instance) {
        return assure<Event>()->on(move(instance));
    }

    /**
     * @brief Registers a short-lived listener with the event Emitter.
     *
     * This method can be used to register a listener designed to be invoked
     * only once for the given event type.<br/>
     * The connection returned by the method can be freely discarded. It's meant
     * to be used later to disconnect the listener if required.
     *
     * The listener is as a callable object that can be moved and the type of
     * which is `void(const Event &, Derived &)`.
     *
     * @note
     * Whenever an event is emitted, the Emitter provides the listener with a
     * reference to the derived class. Listeners don't have to capture those
     * instances for later uses.
     *
     * @tparam Event Type of event to which to connect the listener.
     * @param instance The listener to register.
     * @return Connection object that can be used to disconnect the listener.
     */
    template<typename Event>
    connection<Event> once(listener<Event> instance) {
        return assure<Event>()->once(move(instance));
    }

    /**
     * @brief Disconnects a listener from the event Emitter.
     *
     * Do not use twice the same connection to disconnect a listener, it results
     * in undefined behavior. Once used, discard the connection object.
     *
     * @tparam Event Type of event of the connection.
     * @param conn A valid connection.
     */
    template<typename Event>
    void erase(connection<Event> conn) CG_NOEXCEPT {
        assure<Event>()->erase(move(conn));
    }

    /**
     * @brief Disconnects all the listeners for the given event type.
     *
     * All the connections previously returned for the given event are
     * invalidated. Using them results in undefined behavior.
     *
     * @tparam Event Type of event to reset.
     */
    template<typename Event>
    void clear() CG_NOEXCEPT {
        assure<Event>()->clear();
    }

    /**
     * @brief Disconnects all the listeners.
     *
     * All the connections previously returned are invalidated. Using them
     * results in undefined behavior.
     */
    void clear() CG_NOEXCEPT {
        for_each(handlers.begin(), handlers.end(), [](auto &&hdata) {
            return hdata.handler ? hdata.handler->clear() : void();
        });
    }

    /**
     * @brief Checks if there are listeners registered for the specific event.
     * @tparam Event Type of event to test.
     * @return True if there are no listeners registered, false otherwise.
     */
    template<typename Event>
    bool empty() const CG_NOEXCEPT {
        return assure<Event>()->empty();
    }

    /**
     * @brief Checks if there are listeners registered with the event Emitter.
     * @return True if there are no listeners registered, false otherwise.
     */
    bool empty() const CG_NOEXCEPT {
        return all_of(handlers.cbegin(), handlers.cend(), [](auto &&hdata) {
            return !hdata.handler || hdata.handler->empty();
        });
    }

private:
    mutable vector<handler_data> handlers{};
};


}

