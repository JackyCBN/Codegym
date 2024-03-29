#include "Base.h"
#include "Type/Type.h"

namespace ns_3d
{
	class node
	{
	public:
		node(string name, node* parent = nullptr)
		:m_name(name)
		{
			
		}
		virtual ~node(){}
		void set_name(const string& name)
		{
			m_name = name;
		}
		const string& get_name() const { return m_name; }
		vector<node*> get_children() const
		{
			return m_children;
		}
		void set_visible(bool visible, bool cascade = true){}
		virtual void render(){}

		node* m_parent;
		string         m_name;
		vector<node*>  m_children;
		RTTR_ENABLE()
		//RTTR_REGISTRATION_FRIEND
	};
}


RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace ns_3d;
	registration::class_<node>("ns_3d::node")
		.constructor<string, node*>()
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