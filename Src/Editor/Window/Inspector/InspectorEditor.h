#pragma once

class QWidget;
namespace codegym::editor
{
	class InspectorEditor 
	{
	public:
		virtual void OnInspectorGUI() = 0;

	};
}