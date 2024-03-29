#pragma once

#include "Base.h"
#include "fwd.h"
#include "delegate.h"
#include "EventList.h"


namespace codegym::runtime {


/**
 * @brief Sink class.
 *
 * Primary template isn't defined on purpose. All the specializations give a
 * compile-time error unless the template parameter is a function type.
 *
 * @tparam Function A valid function type.
 */
template<typename Function>
class sink;


/**
 * @brief Unmanaged signal handler.
 *
 * Primary template isn't defined on purpose. All the specializations give a
 * compile-time error unless the template parameter is a function type.
 *
 * @tparam Function A valid function type.
 */
template<typename Function>
class signal;

/**
 * @brief Connection class.
 *
 * Opaque object the aim of which is to allow users to release an already
 * estabilished connection without having to keep a reference to the signal or
 * the sink that generated it.
 */
class connection {
	/*! @brief A sink is allowed to create connection objects. */
	template<typename>
	friend class sink;

	template<typename>
	friend class signal;

	connection(delegate<void(void*)> fn, void* ref)
		: disconnect{ fn }, signal{ ref }
	{}

public:
	/*! Default constructor. */
	connection() = default;

	/*! @brief Default copy constructor. */
	connection(const connection&) = default;

	/**
	 * @brief Default move constructor.
	 * @param other The instance to move from.
	 */
	connection(connection&& other)
		: connection{}
	{
		swap(disconnect, other.disconnect);
		swap(signal, other.signal);
	}

	/*! @brief Default copy assignment operator. @return This connection. */
	connection& operator=(const connection&) = default;

	/**
	 * @brief Default move assignment operator.
	 * @param other The instance to move from.
	 * @return This connection.
	 */
	connection& operator=(connection&& other) {
		if (this != &other) {
			auto tmp{ move(other) };
			disconnect = tmp.disconnect;
			signal = tmp.signal;
		}

		return *this;
	}

	/**
	 * @brief Checks whether a connection is properly initialized.
	 * @return True if the connection is properly initialized, false otherwise.
	 */
	explicit operator bool() const CG_NOEXCEPT {
		return static_cast<bool>(disconnect);
	}

	/*! @brief Breaks the connection. */
	void release() {
		if (disconnect) {
			disconnect(signal);
			disconnect.reset();
		}
	}

private:
	delegate<void(void*)> disconnect;
	void* signal{};
};
	
/**
 * @brief Unmanaged signal handler.
 *
 * It works directly with naked pointers to classes and pointers to member
 * functions as well as pointers to free functions. Users of this class are in
 * charge of disconnecting instances before deleting them.
 *
 * This class serves mainly two purposes:
 *
 * * Creating signals to use later to notify a bunch of listeners.
 * * Collecting results from a set of functions like in a voting system.
 *
 * @tparam Ret Return type of a function type.
 * @tparam Args Types of arguments of a function type.
 */
template<typename Ret, typename... Args>
class signal<Ret(Args...)> {
    /*! @brief A sink is allowed to modify a signal. */
    friend class sink<Ret(Args...)>;

public:
    /*! @brief Unsigned integer type. */
    using size_type = typename vector<delegate<Ret(Args...)>>::size_type;
    /*! @brief Sink type. */
    using sink_type = sink<Ret(Args...)>;

    /**
     * @brief Instance type when it comes to connecting member functions.
     * @tparam Class Type of class to which the member function belongs.
     */
    template<typename Class>
    using instance_type = Class *;

    /**
     * @brief Number of listeners connected to the signal.
     * @return Number of listeners currently connected.
     */
    size_type size() const CG_NOEXCEPT {
        return m_eventList.size();
    }

    /**
     * @brief Returns false if at least a listener is connected to the signal.
     * @return True if the signal has no listeners connected, false otherwise.
     */
    bool empty() const CG_NOEXCEPT {
        return m_eventList.empty();
    }

    /**
     * @brief Triggers a signal.
     *
     * All the listeners are notified. Order isn't guaranteed.
     *
     * @param args Arguments to use to invoke listeners.
     */
    void publish(Args ... args) {

		m_eventList.travel([&](auto&& element) {
			return element.first ? void() : element.second(forward<Args>(args)...);
		});
    }

    /**
     * @brief Collects return values from the listeners.
     *
     * The collector must expose a call operator with the following properties:
     *
     * * The return type is either `void` or such that it's convertible to
     *   `bool`. In the second case, a true value will stop the iteration.
     * * The list of parameters is empty if `Ret` is `void`, otherwise it
     *   contains a single element such that `Ret` is convertible to it.
     *
     * @tparam Func Type of collector to use, if any.
     * @param func A valid function object.
     * @param args Arguments to use to invoke listeners.
     */
    template<typename Func>
	void collect(Func func, Args... args) const {

		m_eventList.travel([&](const auto& element)
		{
			bool stop = false;
			if (element.first)
			{
				if (!element.first)
				{
					if constexpr (is_void_v<Ret>) {
						if constexpr (is_invocable_r_v<bool, Func>) {
							element.second(forward<Args>(args)...);
							stop = func();
						}
						else {
							element.second(forward<Args>(args)...);
							func();
						}
					}
					else {
						if constexpr (is_invocable_r_v<bool, Func, Ret>) {
							stop = func(element.second(forward<Args>(args)...));
						}
						else {
							func(element.second(forward<Args>(args)...));
						}
					}
				}
			}
		});
	}
private:
	using signal_type = signal<Ret(Args...)>;
	template<auto Candidate, typename Type>
	static void release(Type* value_or_instance, void* signal) {
		sink{ *static_cast<signal_type*>(signal) }.disconnect<Candidate>(value_or_instance);
	}

	template<auto Function>
	static void release(void* signal) {
		sink{ *static_cast<signal_type*>(signal) }.template disconnect<Function>();
	}

public:

	/**
	 * @brief Connects a free function to a signal.
	 *
	 * The signal handler performs checks to avoid multiple connections for free
	 * functions.
	 *
	 * @tparam Function A valid free function pointer.
	 * @return A properly initialized connection object.
	 */
	template<auto Function>
	connection connect() {
		disconnect<Function>();
		delegate<void(void*)> conn{};
		conn.template connect<&release<Function>>();
		m_eventList.add(delegate<Ret(Args...)>{connect_arg<Function>});
		return { move(conn), this };
	}

	/**
	 * @brief Connects a member function or a free function with payload to a
	 * signal.
	 *
	 * The signal isn't responsible for the connected object or the payload.
	 * Users must always guarantee that the lifetime of the instance overcomes
	 * the one  of the delegate. On the other side, the signal handler performs
	 * checks to avoid multiple connections for the same function.<br/>
	 * When used to connect a free function with payload, its signature must be
	 * such that the instance is the first argument before the ones used to
	 * define the delegate itself.
	 *
	 * @tparam Candidate Member or free function to connect to the signal.
	 * @tparam Type Type of class or type of payload.
	 * @param value_or_instance A valid pointer that fits the purpose.
	 * @return A properly initialized connection object.
	 */
	template<auto Candidate, typename Type>
	connection connect(Type* value_or_instance) {
		disconnect<Candidate>(value_or_instance);
		delegate<void(void*)> conn{};
		conn.template connect<&signal::template release<Candidate, Type>>(value_or_instance);
		m_eventList.add(delegate<Ret(Args...)>{connect_arg<Candidate>, value_or_instance});
		return { move(conn), this };
	}

	/**
	 * @brief Disconnects a free function from a signal.
	 * @tparam Function A valid free function pointer.
	 */
	template<auto Function>
	void disconnect() {
		auto& calls = m_eventList;
		delegate<Ret(Args...)> delegate{};
		delegate.template connect<Function>();
		calls.erase([delegate = delegate](auto&& elemnt) {
			return elemnt.second == delegate;
		});
	}

	/**
	 * @brief Disconnects a member function or a free function with payload from
	 * a signal.
	 * @tparam Candidate Member or free function to disconnect from the signal.
	 * @tparam Type Type of class or type of payload.
	 * @param value_or_instance A valid pointer that fits the purpose.
	 */
	template<auto Candidate, typename Type>
	void disconnect(Type* value_or_instance) {
		auto& eventList = m_eventList;
		delegate<Ret(Args...)> delegate{};
		delegate.template connect<Candidate>(value_or_instance);

		eventList.erase([delegate = delegate](auto&& elemnt) {
			return elemnt.second == delegate;
		});
	}

	/**
	 * @brief Disconnects member functions or free functions based on an
	 * instance or specific payload.
	 * @param value_or_instance A valid pointer that fits the purpose.
	 */
	void disconnect(const void* value_or_instance) {
		auto& eventList = m_eventList;
		eventList.erase([value_or_instance](auto&& elemnt) {
			const auto& delegate = elemnt.second;
			return value_or_instance == delegate.instance();
		});
	}

	/*! @brief Disconnects all the listeners from a signal. */
	void disconnectAll() {
		m_eventList.clear();
	}

private:
	EventList<delegate<Ret(Args...)>> m_eventList;
};



/**
 * @brief Scoped connection class.
 *
 * Opaque object the aim of which is to allow users to release an already
 * estabilished connection without having to keep a reference to the signal or
 * the sink that generated it.<br/>
 * A scoped connection automatically breaks the link between the two objects
 * when it goes out of scope.
 */
struct scoped_connection: private connection {
    /*! Default constructor. */
    scoped_connection() = default;

    /**
     * @brief Constructs a scoped connection from a basic connection.
     * @param conn A valid connection object.
     */
    scoped_connection(const connection &conn)
        : connection{conn}
    {}

    /*! @brief Default copy constructor, deleted on purpose. */
    scoped_connection(const scoped_connection &) = delete;

    /*! @brief Default move constructor. */
    scoped_connection(scoped_connection &&) = default;

    /*! @brief Automatically breaks the link on destruction. */
    ~scoped_connection() {
        connection::release();
    }

    /**
     * @brief Default copy assignment operator, deleted on purpose.
     * @return This scoped connection.
     */
    scoped_connection & operator=(const scoped_connection &) = delete;

    /**
     * @brief Default move assignment operator.
     * @return This scoped connection.
     */
    scoped_connection & operator=(scoped_connection &&) = default;

    /**
     * @brief Copies a connection.
     * @param other The connection object to copy.
     * @return This scoped connection.
     */
    scoped_connection & operator=(const connection &other) {
        static_cast<connection &>(*this) = other;
        return *this;
    }

    /**
     * @brief Moves a connection.
     * @param other The connection object to move.
     * @return This scoped connection.
     */
    scoped_connection & operator=(connection &&other) {
        static_cast<connection &>(*this) = move(other);
        return *this;
    }

    using connection::operator bool;
    using connection::release;
};


/**
 * @brief Sink class.
 *
 * A sink is used to connect listeners to signals and to disconnect them.<br/>
 * The function type for a listener is the one of the signal to which it
 * belongs.
 *
 * The clear separation between a signal and a sink permits to store the former
 * as private data member without exposing the publish functionality to the
 * users of the class.
 *
 * @tparam Ret Return type of a function type.
 * @tparam Args Types of arguments of a function type.
 */
template<typename Ret, typename... Args>
class sink<Ret(Args...)> {
    using signal_type = signal<Ret(Args...)>;

    template<auto Candidate, typename Type>
    static void release(Type *value_or_instance, void *signal) {
        sink{*static_cast<signal_type *>(signal)}.disconnect<Candidate>(value_or_instance);
    }

    template<auto Function>
    static void release(void *signal) {
        sink{*static_cast<signal_type *>(signal)}.disconnect<Function>();
    }

public:
    /**
     * @brief Constructs a sink that is allowed to modify a given signal.
     * @param ref A valid reference to a signal object.
     */
    sink(signal<Ret(Args...)> &ref) CG_NOEXCEPT
        : signal{&ref}
    {}

    /**
     * @brief Returns false if at least a listener is connected to the sink.
     * @return True if the sink has no listeners connected, false otherwise.
     */
    bool empty() const CG_NOEXCEPT {
        return signal->m_eventList.empty();
    }

    /**
     * @brief Connects a free function to a signal.
     *
     * The signal handler performs checks to avoid multiple connections for free
     * functions.
     *
     * @tparam Function A valid free function pointer.
     * @return A properly initialized connection object.
     */
    template<auto Function>
    connection connect() {

		return signal->connect<Function>();
    }

    /**
     * @brief Connects a member function or a free function with payload to a
     * signal.
     *
     * The signal isn't responsible for the connected object or the payload.
     * Users must always guarantee that the lifetime of the instance overcomes
     * the one  of the delegate. On the other side, the signal handler performs
     * checks to avoid multiple connections for the same function.<br/>
     * When used to connect a free function with payload, its signature must be
     * such that the instance is the first argument before the ones used to
     * define the delegate itself.
     *
     * @tparam Candidate Member or free function to connect to the signal.
     * @tparam Type Type of class or type of payload.
     * @param value_or_instance A valid pointer that fits the purpose.
     * @return A properly initialized connection object.
     */
    template<auto Candidate, typename Type>
    connection connect(Type *value_or_instance) {
		return signal->connect<Candidate>(value_or_instance);
    }

    /**
     * @brief Disconnects a free function from a signal.
     * @tparam Function A valid free function pointer.
     */
    template<auto Function>
    void disconnect() {
		return signal->template disconnect<Function>();
    }

    /**
     * @brief Disconnects a member function or a free function with payload from
     * a signal.
     * @tparam Candidate Member or free function to disconnect from the signal.
     * @tparam Type Type of class or type of payload.
     * @param value_or_instance A valid pointer that fits the purpose.
     */
    template<auto Candidate, typename Type>
    void disconnect(Type *value_or_instance) {
		return signal->disconnect<Candidate>(value_or_instance);
    }

    /**
     * @brief Disconnects member functions or free functions based on an
     * instance or specific payload.
     * @param value_or_instance A valid pointer that fits the purpose.
     */
    void disconnect(const void *value_or_instance) {
		return signal->disconnect(value_or_instance);
    }

    /*! @brief Disconnects all the listeners from a signal. */
    void disconnect() {
        signal->disconnectAll();
    }

private:
    signal_type *signal;
};


/**
 * @brief Deduction guide.
 *
 * It allows to deduce the function type of a sink directly from the signal it
 * refers to.
 *
 * @tparam Ret Return type of a function type.
 * @tparam Args Types of arguments of a function type.
 */
template<typename Ret, typename... Args>
sink(signal<Ret(Args...)> &) CG_NOEXCEPT -> sink<Ret(Args...)>;


}