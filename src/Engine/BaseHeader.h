﻿#pragma once

#pragma warning(push, 3)
#pragma warning(disable : 4061)
#pragma warning(disable : 4365)
#pragma warning(disable : 4668)
#pragma warning(disable : 4820)
#pragma warning(disable : 5039)
#pragma warning(disable : 5219)

#define _USE_MATH_DEFINES

#include <cassert>
#include <string>
#include <array>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOBITMAP
#define NOGDICAPMASKS
//#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define NOGDI
#define NOWINSTYLE
#define NOKEYSTATES
#define NOCOLOR
#define NOMB
//#define NOWINOFFSETS
#define NOCRYPT
#define NOIME

#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00 // Windows 10
#include <sdkddkver.h>
#include <Windows.h>

#include <wrl/client.h>

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#if defined(_DEBUG)
#	include <dxgidebug.h>
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>

#pragma warning(pop)