#include <thread>
#include <fstream>
#include <string>
#include "minhook/MinHook.h"
#include "sdk.h"

CMaterialSystem* material_system;
CModelRender* model_render;
CClientEntityList* ClientEntityList;
CModelInfoClient* model_info;
CStudioRenderContext* studio_render;
CVRenderView* render_view;
IVEngineClient* engine;
CMatSystemSurface* surface;
CPlayerInfoManager* player_info;
CEngineVGui* engine_vgui;
CPanel* panel;

typedef void(__thiscall* SceneEndFn)(void*);
SceneEndFn SceneEndRet;

typedef void(__thiscall* Vgui_PaintFn)( unsigned int, bool, bool);
Vgui_PaintFn Vgui_PaintRet;

CMaterial* mat = 0;
float color[] = {1.f, 0.f, 0.f};

void SceneEndHK(void* a1) {
    SceneEndRet(a1);

    if (mat) {
        auto localplayer = reinterpret_cast<Player_t*>(ClientEntityList->GetClientEntity(engine->GetLocalPlayer()));
        for (int i = 0; i < ClientEntityList->GetHighestEntityIndex(); i++) {
            auto player = ClientEntityList->GetClientEntity(i)->as<Player_t>();
            if (!player) continue;
            if (!player->IsPlayer() and !player->IsC4()) continue;
            if (player->IsDormant()) continue;
            if (player == localplayer) continue;

            render_view->SetBlend(0.5f);
            render_view->SetColorModulation(color);
            model_render->ForcedMaterialOverride(mat);

            player->DrawModel(STUDIO_RENDER);
        }
        model_render->ForcedMaterialOverride(nullptr);
    }
    else {
       /* std::ofstream("cstrike\\materials\\jesusloves.vmt") << R"#("VertexLitGeneric" {
	        $additive				1
	        $envmap					"models/effects/cube_white"
	        $envmaptint				"[0 0 0]"
	        $envmapfresnel			1
	        $envmapfresnelminmaxexp "[0 16 12]"
	        $alpha					0.8
        })#";*/

        //mat = material_system->FindMaterial("jesusloves", "Model textures", true, 0);      
        mat = material_system->FindMaterial("//platform/materials/debug/debugmrmwireframe", "Model textures", true, 0);      
        if (!mat) SceneEndRet(a1);
        mat->IncrementReferenceCount();
        mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
    }   
}

void Vgui_Paint( unsigned int a1, bool a2, bool a3) {
    //if (a2 & 1) {
    //}
    Vgui_PaintRet( a1, a2, a3);
    surface->DrawLine(0,0,500,500);
}

void main(HINSTANCE hDLL) {
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    render_view = memory::GetInterface<CVRenderView*>("engine.dll", "VEngineRenderView014");
    engine = memory::GetInterface<IVEngineClient*>("engine.dll", "VEngineClient014");
    model_render = memory::GetInterface<CModelRender*>("engine.dll", "VEngineModel016");
    material_system = memory::GetInterface<CMaterialSystem*>("materialsystem.dll", "VMaterialSystem082");
    model_info = memory::GetInterface<CModelInfoClient*>("engine.dll", "VModelInfoClient006");
    engine_vgui = memory::GetInterface<CEngineVGui*>("engine.dll", "VEngineVGui002");
    ClientEntityList = memory::GetInterface<CClientEntityList*>("client.dll", "VClientEntityList003");
    studio_render = memory::GetInterface<CStudioRenderContext*>("studiorender.dll", "VStudioRender025");
    surface = memory::GetInterface<CMatSystemSurface*>("vguimatsurface.dll", "VGUI_Surface030");
    player_info = memory::GetInterface<CPlayerInfoManager*>("server.dll", "PlayerInfoManager002");
    panel = memory::GetInterface<CPanel*>("vgui2.dll", "VGUI_Panel009");

    MH_Initialize();
    MH_CreateHook(memory::GetVtable(render_view)[9], &SceneEndHK, (void**)&SceneEndRet);
    MH_EnableHook(memory::GetVtable(render_view)[9]);

    /*MH_CreateHook(memory::GetVtable(panel)[41], &Vgui_Paint, (void**)&Vgui_PaintRet);
    MH_EnableHook(memory::GetVtable(panel)[41]);*/

    for(;;) {
        if (GetAsyncKeyState(VK_DELETE) & 0x1) break;
    }

    printf("Unhooked!\n");
    MH_DisableHook(MH_ALL_HOOKS);
    FreeLibraryAndExitThread(hDLL, 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH: 
        std::thread(main, hinstDLL).detach(); 
        break;
    case DLL_PROCESS_DETACH:
        if (lpvReserved != nullptr) break;
        break;
    }
    return TRUE;
}