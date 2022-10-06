// 2022/04/29 14:52:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#define WIN32_LEAN_AND_MEAN


typedef const char *pchar;
typedef unsigned char uchar;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;
typedef signed char int8;


#include "Log.h"

#pragma warning(push, 0)
#include <list>
#include <bitset>
#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/grid.h>
#include <wx/dirctrl.h>
#include <wx/notebook.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/zstream.h>
#include <wx/mstream.h>
#include <wx/dir.h>
#include <wx/wfstream.h>
#include <wx/xml/xml.h>
#include <wx/xtixml.h>
#include <glm/glm.hpp>
#pragma warning(pop)
