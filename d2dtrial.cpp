#include "d2dtrial.h"
#include<string>
#define DPI_DEFAULT 96;

dxresources::~dxresources()
{
    Release();
}

void dxresources::init()
{
    d3dinit();
    d2dinit();
    dwriteinit();
    initdftaprcrsrc();
    initbmrt();
}

void dxresources::d3dinit()
{
    auto runflag =
        D3D11_CREATE_DEVICE_SINGLETHREADED |
        // https://learn.microsoft.com/en-us/windows/win32/direct3d11/using-the-debug-layer-to-test-apps
#ifdef D3DDEBUGLAYER
        D3D11_CREATE_DEVICE_DEBUG |
#endif // D3DDEBUGLAYER       
        D3D11_CREATE_DEVICE_BGRA_SUPPORT
        ;
    D3D_FEATURE_LEVEL levels[7] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    sdx::dxerr(
        D3D11CreateDevice(
            NULL,
            D3D_DRIVER_TYPE_HARDWARE,//D3D_DRIVER_TYPE_WARP
            NULL,
            runflag,
            levels,
            1,//start with level 11_0
            D3D11_SDK_VERSION,
            &d3ddvc,
            NULL,
            &d3ddc
        )
    );
}

void dxresources::d2dinit()
{
    /*typedef enum D2D1_DEBUG_LEVEL {
        D2D1_DEBUG_LEVEL_NONE = 0,
        D2D1_DEBUG_LEVEL_ERROR = 1,
        D2D1_DEBUG_LEVEL_WARNING = 2,
        D2D1_DEBUG_LEVEL_INFORMATION = 3,
        D2D1_DEBUG_LEVEL_FORCE_DWORD = 0xffffffff
    };*/
    D2D1_FACTORY_OPTIONS options;

#ifdef D2DDEBUGLAYER
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
    options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

    sdx::dxerr(
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            //D2D1_FACTORY_TYPE_MULTI_THREADED,
            __uuidof(D2DFct),
            &options,
            (void**)&d2dfct
        )
    );

    sdx::dxerr(
        d3ddvc->QueryInterface(__uuidof(DXGIdvc), (void**)&dxgidvc)
    );
    sdx::dxerr(
        d2dfct->CreateDevice(dxgidvc, &d2ddvc)
    );

    sdx::dxerr(
        d2ddvc->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS
            //D2D1_DEVICE_CONTEXT_OPTIONS_NONE
            , &d2ddc
        )
    );


}

void dxresources::initdftaprcrsrc()
{
    d2ddc->CreateSolidColorBrush(
        D2D1::ColorF(0x1e4aFF),
        &scbrush
    );
    wfct->CreateTextFormat(
        L"Msyh",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        12.0f,
        L"zh-CN",
        &dwtfm
    );
}

void dxresources::initbmrt()
{
    auto w = rc.right - rc.left;
    auto h = rc.bottom - rc.top;
    sdx::dxerr(
        d2ddc->CreateCompatibleRenderTarget(
            D2D1::SizeF(w, h),
            D2D1::SizeU(w, h),
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, D2D1_ALPHA_MODE_PREMULTIPLIED),
            D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE,
            &d2dbmrt
        )
    );
}

void dxresources::dwriteinit()
{
    sdx::dxerr(DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&wfct)
    )
    );
}



void dxresources::bindwnd(HWND hwnd)
{
    this->hwnd = hwnd;
    updaterect();
    // Allocate a descriptor.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

    swapChainDesc.Width = 0;// rrc.right - rrc.left;//set 0 to use automatic sizing
    swapChainDesc.Height = 0;// rrc.bottom - rrc.top;
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;//ÏñËØ¸ñÊ½ // this is the most common swapchain format
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1; // don't use multi-sampling
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
    swapChainDesc.Scaling = DXGI_SCALING_NONE;//DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all apps must use this SwapEffect
    swapChainDesc.Flags = 0; //DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


    IDXGIAdapter* dxgiAdapter;
    sdx::dxerr(
        dxgidvc->GetAdapter(&dxgiAdapter)
    );

    IDXGIFactory2* dxgifct;
    sdx::dxerr(
        dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgifct))
    );

    sdx::dxerr(
        dxgifct->CreateSwapChainForHwnd(
            d3ddvc,
            hwnd,
            &swapChainDesc,
            NULL,
            NULL, 
            &swapchain
        )
    );
    
    sdx::dxerr(
        dxgidvc->SetMaximumFrameLatency(1)
    );

    auto dpi_hwnd = GetDpiForWindow(hwnd);
    /*
    * OPTIONS
        ID2D1DeviceContext£º£ºSetTarget()-->D2D1_BITMAP_OPTIONS_TARGET
        ID2D1Bitmap1£º£ºMap()-->D2D1_BITMAP_OPTIONS_CPU_READ

      PIXFORMAT
        ref£º
        https://learn.microsoft.com/zh-cn/windows/win32/direct2d/supported-pixel-formats-and-alpha-modes#supported-formats-for-id2d1devicecontext
        DXGI_FORMAT_B8G8R8A8_UNORM<=>(D2D1_ALPHA_MODE_IGNORE|D2D1_ALPHA_MODE_PREMULTIPLIED)
    */
    D2D1_BITMAP_PROPERTIES1 bitmapProperties =
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            dpi_hwnd,
            dpi_hwnd
        );

    IDXGISurface* dxgiBackBuffer;
    sdx::dxerr(
        swapchain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
    );

    // Get a D2D surface from the DXGI back buffer to use as the D2D render target.
    sdx::dxerr(d2ddc->CreateBitmapFromDxgiSurface(
        dxgiBackBuffer,
        &bitmapProperties,
        &bm1
    ));

    d2ddc->SetTarget(bm1);

    /*D2D1_RENDERING_CONTROLS drc;
    d2ddc->GetRenderingControls(&drc);
    drc.bufferPrecision = D2D1_BUFFER_PRECISION_8BPC_UNORM;
    d2ddc->SetRenderingControls(drc);*/

    //d2ddc->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    //d2ddc->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    d2ddc->SetDpi(dpi_hwnd, dpi_hwnd);
}

void dxresources::updaterect()
{
    GetWindowRect(hwnd, &rc);
    dpirate = GetDpiForWindow(hwnd) * 1.0f / DPI_DEFAULT;
    //dpirate = 1;
    rrc = RECT();
    rrc.top = rc.top / dpirate;
    rrc.bottom = rc.bottom / dpirate;
    rrc.left = rc.left / dpirate;
    rrc.right = rc.right / dpirate;
    lrc.top = lrc.left = 0;
    lrc.right = rrc.right - rrc.left;
    lrc.bottom = rrc.bottom - rrc.top;


}
void dxresources::updaterc()
{
    updaterect();
}

void dxresources::render_exp()
{
    d2ddc->BeginDraw();
    d2ddc->Clear(D2D1::ColorF(0x1e4a56));
    ID2D1RectangleGeometry* fg;
    d2dfct->CreateRectangleGeometry(D2D1::RectF(300, 100, 350, 150), &fg);
    d2ddc->FillGeometry(fg, scbrush);

    auto rct = D2D1::RectF(50, 50, 100, 100);
    d2ddc->FillRectangle(rct, scbrush);

    rct = D2D1::RectF(0, 0, rc.right, rc.bottom);
    d2ddc->DrawRectangle(rct, scbrush);
    
    auto dwstr = L"adsdas^&*$*(d34123qwe";
    D2D1_RECT_F layoutRect = D2D1::RectF(0.f, 0.f, 400.f, 100.f);
    d2ddc->DrawTextW(
        dwstr,
        wcslen(dwstr),
        dwtfm,
        layoutRect,
        scbrush
    );

    sdx::dxerr(
        d2ddc->EndDraw()
    );

    sdx::dxerr(
        swapchain->Present1(1, 0, &dxgiscp)
    );

}


void dxresources::begindraw()
{
    d2ddc->BeginDraw();
    d2ddc->Clear(D2D1::ColorF(0x1e4a56));
}

void dxresources::enddraw()
{
    sdx::dxerr(
        d2ddc->EndDraw()
    );
 
    /*DXGI_SWAP_CHAIN_DESC1 scd;
    swapchain->GetDesc1(&scd);*/
    //sdx::dxerr(swapchain->Present1(1, 0, &dxgiscp));
}

void dxresources::sendpaintmsg()
{
    RedrawWindow(hwnd, &rrc, NULL, RDW_INTERNALPAINT);
}

void dxresources::present()
{
    sdx::dxerr(swapchain->Present1(1, 0, &dxgiscp));

}

void dxresources::Release()
{


    dxrelease(dwtfm);
    dxrelease(scbrush);
    dxrelease(wfct);
    dxrelease(d2dfct);
    dxrelease(bm1);
    dxrelease(swapchain);
    dxrelease(d2ddc);
    dxrelease(d2ddvc);
    dxrelease(dxgidvc);
    dxrelease(d3ddc);
    dxrelease(d3ddvc);
}

