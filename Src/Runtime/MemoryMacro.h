#pragma once


#define SAFE_DELETE(ptr) while (false){ if(ptr) {delete (ptr); (ptr) = nullptr;}};
