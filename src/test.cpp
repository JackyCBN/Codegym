#include <QApplication>
#include <QMainWindow>
#include <rttr/type>
#include <rttr/registration>
#include <iostream>

namespace ns_3d
{
	class node
	{
	public:
		node(std::string name, node* parent = nullptr)
		:m_name(name)
		{
			
		}
		virtual ~node(){}
		void set_name(const std::string& name)
		{
			m_name = name;
		}
		const std::string& get_name() const { return m_name; }
		std::vector<node*> get_children() const
		{
			return m_children;
		}
		void set_visible(bool visible, bool cascade = true){}
		virtual void render(){}

		node* m_parent;
		std::string         m_name;
		std::vector<node*>  m_children;
		RTTR_ENABLE()
		//RTTR_REGISTRATION_FRIEND
	};
}


RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace ns_3d;
	registration::class_<node>("ns_3d::node")
		.constructor<std::string, node*>()
		(
			policy::ctor::as_std_shared_ptr, // should create an instance of the class as shared_ptr<ns_3d::node>
			default_arguments(nullptr)       // second argument is optional, so we provide the default value for it
		)
		.property("name", &node::get_name, &node::set_name)
		(
			metadata("TOOL_TIP", "Set the name of node.")  // stores metadata associated with this property
		)
		// register directly a member object pointer; mark it as 'private'
		.property("parent", &ns_3d::node::m_parent, registration::private_access)
		.property_readonly("children", &node::get_children) // a read-only property; so not set possible
		.method("set_visible", &node::set_visible)
		(
			default_arguments(true),              // the default value for 'cascade'
			parameter_names("visible", "cascade") // provide the names of the parameter; optional, but might be useful for clients
		)
		.method("render", &node::render)
		;
}
//int main(int argc, char **argv)
//{
// QApplication app (argc, argv);
// QMainWindow mw;
// mw.show();
//
// return app.exec();
//}

int main(int argc, char* argv[])
{
	using namespace rttr;
	type t = type::get_by_name("ns_3d::node");
	// will create an instance of ns_3d::node as std::shared_ptr<ns_3d::node>
	variant var = t.create({ std::string("MyNode") });
	std::cout << var.get_type().get_name() << "\n";
	// sets/gets a property
	property prop = t.get_property("name");
	// remark: you can also set a member, although the instance is of type: 'std::shared_ptr<T>'
	prop.set_value(var, std::string("A New Name"));
	std::cout << prop.get_value(var).to_string() << "\n";
	// retrieve the stored meta data of the property
	std::cout << "MetaData TOOL_TIP: " << prop.get_metadata("TOOL_TIP").to_string() << "\n";
	// invoke a method
	method meth = t.get_method("set_visible");
	// remark: the 2nd argument will be provided automatically, because it has a default argument
	variant ret = meth.invoke(var, true);
	// a valid return value indicates a successful invoke
	std::cout << std::boolalpha << "invoke of method 'set_visible' was successfully: " << ret.is_valid() << "\n\n";
	// retrieve all properties
	std::cout << "'node' properties:" << "\n";
	for (auto& prop : t.get_properties())
	{
		std::cout << "  name: " << prop.get_name() << "\n";
		std::cout << "    type: " << prop.get_type().get_name() << "\n";
	}
	std::cout << "\n";
	// retrieve all methods
	std::cout << "'node' methods:" << "\n";
	for (auto& meth : t.get_methods())
	{
		std::cout << "  name: " << meth.get_name();
		std::cout << "  signature: " << meth.get_signature() << "\n";
		for (auto& info : meth.get_parameter_infos())
		{
			std::cout << "    param " << info.get_index() << ": name: " << info.get_name() << "\n";
		}
	}
	return 0;
}