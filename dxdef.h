#pragma once
#ifndef DXDEF_H
#define DXDEF_H

#include "d2d1_3.h"
#pragma comment(lib,"D2d1.lib")
#include "dwrite_3.h"
#pragma comment(lib,"Dwrite.lib")
#include "d3d11_4.h"
#pragma comment(lib,"D3d11.lib")
#include "dxgi1_6.h"

#include <stdexcept>

#define DXDEBUG

namespace sdx {
	inline void dxerr(HRESULT hr) {
#if defined DXDEBUG &&defined _DEBUG
		if (FAILED(hr))throw std::runtime_error("");;
#endif
	};
}
//#define D3DDEBUG
#if defined D3DDEBUG&& defined DXDEBUG
#define D3DDEBUGLAYER
#endif

//#define D2DDEBUG
#if defined D2DDEBUG&& defined DXDEBUG
#define D2DDEBUGLAYER
#endif


typedef ID2D1Factory7 D2DFct;
typedef ID2D1Device6 D2DDvc;
typedef IDXGIDevice4 DXGIdvc;
typedef ID2D1DeviceContext6 D2DDC;
typedef ID2D1DeviceContext6 D2DRT;
typedef ID2D1BitmapRenderTarget D2DBMRT;
typedef IDXGISwapChain1 DXGISC1;
typedef IDWriteFactory7 WFCT;
typedef ID2D1Bitmap1 D2DBM1;
typedef ID2D1Bitmap D2DBM;
typedef ID2D1GeometryGroup D2DGG;
typedef IDWriteTextFormat DWTFM;
typedef ID2D1SolidColorBrush D2DSCB;
typedef ID2D1BitmapBrush1 D2DBMBRS;
typedef ID2D1Resource D2DRsrc;
typedef ID2D1Geometry  D2DGeo;

#endif // !DXDEF_H

