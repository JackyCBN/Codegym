#pragma once

typedef void StartUpFunc();
void AddStartup(StartUpFunc* func);
void ExecuteStartups();

#define STARTUP(a) struct STARTUP##a { STARTUP##a() {AddStartup(&(a));} }; static STARTUP##a gSTARTUP##a##__LINE__;