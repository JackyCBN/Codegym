#pragma once
#include "QtPropertyBrowser/qttreepropertybrowser.h"

class QWidget;
namespace codegym::editor
{
	class InspectorWindow : public QtTreePropertyBrowser
	{
		Q_OBJECT
	public:
		InspectorWindow(QWidget* parent = nullptr);

	};
}