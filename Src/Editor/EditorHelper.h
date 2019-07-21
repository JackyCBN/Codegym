#pragma once

typedef void StartUpFunc();
void AddStartup(StartUpFunc* func);
void ExecuteStartups();
#define MENU_STARTUP(a) struct MENU_STARTUP##a { MENU_STARTUP##a() {AddStartup(&a);} }; static MENU_STARTUP##a gMENU_STARTUP##a##__LINE__;