#pragma once
#ifndef D2DPAINTER_H
#define D2DPAINTER_H

#include "dxdef.h"

template<typename T>
void dxrelease(T*& rsrc) {
	rsrc->Release();
	rsrc = NULL;
};

class dxresources;
typedef dxresources dxpainter;
class dxresources {

	RECT rc;
	RECT rrc;
	RECT lrc;

	ID3D11Device* d3ddvc;
	ID3D11DeviceContext* d3ddc;
	DXGIdvc* dxgidvc;
	DXGI_PRESENT_PARAMETERS dxgiscp;

	D2DDvc* d2ddvc;
	D2DFct* d2dfct;
	D2DDC* d2ddc;
	D2DBMRT* d2dbmrt;

	WFCT* wfct;

	DWTFM* dwtfm = NULL;
	D2DSCB* scbrush = NULL;

	DXGISC1* swapchain;
	D2DBM1* bm1;
	ID3D11RenderTargetView* rtv;
	D2DBM1* bm1_pre;

	boolean drawerneed;


	void d3dinit();
	void d2dinit();
	void dwriteinit();

	void initdftaprcrsrc();

	void initbmrt();

public:
	HWND hwnd;

	float dpirate;


	~dxresources();
	void init();
	void bindwnd(HWND hwnd);

	void updaterect();
	void updaterc();

	void render_exp();

	RECT getrrc() { return rrc; };
	
	float getdpirate() { return dpirate; };

	D2DDC* getd2ddc() { return d2ddc; };
	D2DSCB* getd2dscb() { return scbrush; };
	DWTFM* gettextformat() { return dwtfm; };
	D2DFct* getd2dfct() { return d2dfct; };
	WFCT* getwfct() { return wfct; };
	HWND gethwnd() { return hwnd; };

	void begindraw();
	void enddraw();
	void present();

	void sendpaintmsg();
	void Release();
};

#endif