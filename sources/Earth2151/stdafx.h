// 2025/01/05 23:13:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#include <System/C4Application.h>
#include <System/C4Engine.h>
#include <World/C4World.h>
#include <World/C4Cameras.h>
#include <World/C4Models.h>
#include <System/C4Input.h>
#include <System/C4Main.h>
#include <Interface/C4Interface.h>
#include <Interface/C4Configuration.h>
#include <Effects/C4Markings.h>
#include <Geometry/C4Terrain.h>
#include <Effects/C4Particles.h>
#include <Math/C4Random.h>
#include <Resource/C4ConfigData.h>
#include <Interface/C4Viewports.h>


#include <chrono>
#include <filesystem>
#include <array>


using namespace Pi;
using namespace Terathon;


extern bool g_showing_gizmo;    // Если true, то надо показывать gizmo для всех объектов


#define SAFE_DELETE(x) { if(x) { delete x; x = nullptr; } }
#define SAFE_DELETE_ARRAY(x) { if(x) { delete [](x); x = nullptr ;}}


typedef unsigned long ulong;


#ifdef WIN32
#define _(x) u8##x
#else
#define _(x) x
#endif

