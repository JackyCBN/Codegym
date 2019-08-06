#pragma once
#include "QtPropertyBrowser/qttreepropertybrowser.h"

class QWidget;
namespace codegym::editor
{
	class InspectorEditor;

	class InspectorWindow : public QtTreePropertyBrowser
	{
		Q_OBJECT
	public:
		InspectorWindow(QWidget* parent = nullptr);

	private:
		std::vector<InspectorEditor*> m_editors;

	};
}