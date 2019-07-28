#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);

	QMenuBar* GetMenubar() const;

public slots:
	void clickMenuItem();
private:
	QMenuBar* m_menubar = nullptr;
};
