#pragma once


#define SAFE_DELETE(ptr) do{ if(ptr) {delete (ptr); (ptr) = nullptr;}}while (false);
