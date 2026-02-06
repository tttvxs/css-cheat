#include <thread>
#include <fstream>
#include <string>
#include "minhook/MinHook.h"
#include "sdk.h"
#include <format>
#include <vector>
#include <unordered_set>

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

typedef void(__thiscall* SceneEndFn)(void*);
SceneEndFn SceneEndRet;

typedef void(__thiscall* Vgui_PaintFn)(void* a1, int mode);
Vgui_PaintFn Vgui_PaintRet;

CMaterial* mat = 0;
float color[] = {1.f, 0.f, 0.f};
float color2[] = {0.f, 1.f, 0.f};

inline VMatrix m_WorldToProjection = {}; // w2s
inline int screen_width = 0;
inline int screen_height = 0;

bool w2s(vec3& vOrigin, vec3& vScreen) {
    const matrix3x4& worldToScreen = m_WorldToProjection.As3x4();
    float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3];

    if (w > 0.001) {
        vScreen.x = (screen_width / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * (1 / w)) * screen_width + 0.5);
        vScreen.y = (screen_height / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * (1 / w)) * screen_height + 0.5);
        return true;
    }

    return false;
}

void UpdateW2S() {
    CViewSetup ViewSetup;

    if ((bool)(chlclient->GetPlayerView(ViewSetup))) {
        VMatrix WorldToView, ViewToProjection, WorldToPixels;
        render_view->GetMatricesForView(ViewSetup, &WorldToView, &ViewToProjection, &m_WorldToProjection, &WorldToPixels);
    }
}

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
            bool isTeammate = player->m_iTeamNum() == localplayer->m_iTeamNum();

            render_view->SetBlend(0.4f);
            if(isTeammate)render_view->SetColorModulation(color);
            else render_view->SetColorModulation(color2);
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

void Vgui_Paint(void* a1, int mode) {
    if (!screen_height || !screen_width) {
        screen_height = (int)chlclient->GetScreenHeight();
        screen_width =  (int)chlclient->GetScreenWidth();
    }
    if (mode & 1) {
        UpdateW2S();
        if (ClientEntityList) {
            auto localplayer = reinterpret_cast<Player_t*>(ClientEntityList->GetClientEntity(engine->GetLocalPlayer()));
            for (int i = 0; i < ClientEntityList->GetHighestEntityIndex(); i++) {
                auto pEntity = ClientEntityList->GetClientEntity(i)->as<Player_t>();
                if (!pEntity) continue;
                if (!pEntity->IsPlayer()) continue;
                if (pEntity->m_iTeamNum() == localplayer->m_iTeamNum()) continue;

                auto pos = pEntity->m_vecOrigin();
                vec3 scr;
                if (w2s(pos, scr)) {
                    surface->DrawSetColor(255, 0, 0, 255);
                    surface->DrawLine(screen_width / 2, 0, scr.x, scr.y);
                }
            }
        }
    }
    Vgui_PaintRet(a1, mode);
}

void main(HINSTANCE hDLL) {
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr); 

    render_view = memory::GetInterface<CVRenderView*>("engine.dll", "VEngineRenderView014");
    chlclient = memory::GetInterface<CHLClient*>("client.dll", "VClient017");
    engine = memory::GetInterface<IVEngineClient*>("engine.dll", "VEngineClient014");
    model_render = memory::GetInterface<CModelRender*>("engine.dll", "VEngineModel016");
    material_system = memory::GetInterface<CMaterialSystem*>("materialsystem.dll", "VMaterialSystem082");
    model_info = memory::GetInterface<CModelInfoClient*>("engine.dll", "VModelInfoClient006");
    engine_vgui = memory::GetInterface<CEngineVGui*>("engine.dll", "VEngineVGui002");
    ClientEntityList = memory::GetInterface<CClientEntityList*>("client.dll", "VClientEntityList003");
    studio_render = memory::GetInterface<CStudioRenderContext*>("studiorender.dll", "VStudioRender025");
    surface = memory::GetInterface<CMatSystemSurface*>("vguimatsurface.dll", "VGUI_Surface030");
    player_info = memory::GetInterface<CPlayerInfoManager*>("server.dll", "PlayerInfoManager002");

    //netvars_dump();

    MH_Initialize();
    MH_CreateHook(memory::GetVtable(render_view)[9], &SceneEndHK, (void**)&SceneEndRet);
    MH_EnableHook(memory::GetVtable(render_view)[9]);

    MH_CreateHook(memory::GetVtable(engine_vgui)[14], &Vgui_Paint, (void**)&Vgui_PaintRet);
    MH_EnableHook(memory::GetVtable(engine_vgui)[14]);

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