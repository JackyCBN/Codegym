#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	void CreateHierarchy();
	void CreateProperties();
	void CreateOutput();

	void InitLayout();
	QMenuBar* GetMenubar() const;

public slots:
	void clickMenuItem();
private:
	QMenuBar* m_menubar = nullptr;
};
