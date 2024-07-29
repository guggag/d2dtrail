# d2dtrail
The key content is in the InitInstance(...) of exp.cpp

1.Init dx resources.

    dxrsrc = new dxresources();
    dxrsrc->init();
    dxrsrc->bindwnd(hWnd);
    dxrsrc->updaterect();
    
2.A loop used to repeate the rendering process.

    std::thread([]() 
        {
            for (auto i=0;;++i) {
                Sleep(800);
                std::cout << "render " << i << std::endl;
                 dxrsrc->render_exp();
            }
        }
    ).detach();

The flickering issue does not always occur during every rendering process.
