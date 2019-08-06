#pragma once
#include "InspectorEditor.h"

class QWidget;
namespace codegym::editor
{
	class DefaultInspector : InspectorEditor
	{
	public:
		void OnInspectorGUI() override;

	};
}