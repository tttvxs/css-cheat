#pragma once
#include <cstdint>
#include <Windows.h>
#include "utils/Vectors.h"
#include "utils/memory.h"
#include "../../../TF2/TF2/sdk/dt/dt_recv.h"

class CBaseHandle;
class IClientRenderable;
class CStudioHdr;
class Ray_t;
class IWorldRenderList;
class VPlane;
class VisibleFogVolumeInfo_t;
class ITexture;
class CSaveRestoreData;
class CEngineSprite;
class IConVar;
struct model_t;
struct ClientFrameStage_t;
struct dlight_t;
struct WorldListInfo_t;
struct ScreenFade_t;
struct VisOverrideData_t;
struct ButtonCode_t;
struct datamap_t;
struct IBrushRenderer;
typedef unsigned short MaterialHandle_t;
class IClientNetworkable;
class C_BaseEntity;
class ICollideable;
class IClientThinkable;
class IClientEntity;
class IClientModelRenderable;
class IClientAlphaProperty;
class c_client_think_handle_ptr;
typedef c_client_think_handle_ptr* client_think_handle_t;
class SurfInfo;
class CAudioSource;
struct client_textmessage_t;
struct StartSoundParams_t;
struct typedescription_t;
class CSentence;
class IFileList;
class ISpatialQuery;
class ClientClass;
extern ClientClass* g_pClientClassHead;
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100	// bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT		0x00000200	// bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400	// bone (or child) is used by the toplevel model via skinned vertex

#define STUDIO_NONE						0x00000000
#define STUDIO_RENDER					0x00000001
#define STUDIO_VIEWXFORMATTACHMENTS		0x00000002
#define STUDIO_DRAWTRANSLUCENTSUBMODELS 0x00000004
#define STUDIO_TWOPASS					0x00000008
#define STUDIO_STATIC_LIGHTING			0x00000010
#define STUDIO_WIREFRAME				0x00000020
#define STUDIO_ITEM_BLINK				0x00000040
#define STUDIO_NOSHADOWS				0x00000080
#define STUDIO_WIREFRAME_VCOLLIDE		0x00000100
#define STUDIO_NO_OVERRIDE_FOR_ATTACH	0x00000200
#define STUDIO_GENERATE_STATS			0x01000000
#define STUDIO_SSAODEPTHTEXTURE			0x08000000
#define STUDIO_SHADOWDEPTHTEXTURE		0x40000000
#define STUDIO_TRANSPARENCY				0x80000000

enum MaterialVarFlags_t {
    MATERIAL_VAR_DEBUG = (1 << 0),
    MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
    MATERIAL_VAR_NO_DRAW = (1 << 2),
    MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

    MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
    MATERIAL_VAR_VERTEXALPHA = (1 << 5),
    MATERIAL_VAR_SELFILLUM = (1 << 6),
    MATERIAL_VAR_ADDITIVE = (1 << 7),
    MATERIAL_VAR_ALPHATEST = (1 << 8),
    //	MATERIAL_VAR_UNUSED					  = (1 << 9),
    MATERIAL_VAR_ZNEARER = (1 << 10),
    MATERIAL_VAR_MODEL = (1 << 11),
    MATERIAL_VAR_FLAT = (1 << 12),
    MATERIAL_VAR_NOCULL = (1 << 13),
    MATERIAL_VAR_NOFOG = (1 << 14),
    MATERIAL_VAR_IGNOREZ = (1 << 15),
    MATERIAL_VAR_DECAL = (1 << 16),
    MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
    //	MATERIAL_VAR_UNUSED					  = (1 << 18),
    MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
    MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
    MATERIAL_VAR_TRANSLUCENT = (1 << 21),
    MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
    MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
    MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
    MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
    MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
    MATERIAL_VAR_HALFLAMBERT = (1 << 27),
    MATERIAL_VAR_WIREFRAME = (1 << 28),
    MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
    MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
    MATERIAL_VAR_VERTEXFOG = (1 << 31),

    // NOTE: Only add flags here that either should be read from
    // .vmts or can be set directly from client code. Other, internal
    // flags should to into the flag enum in IMaterialInternal.h
};

enum OverrideType_t : std::int32_t {
    OVERRIDE_NORMAL = 0,
    OVERRIDE_BUILD_SHADOWS,
    OVERRIDE_DEPTH_WRITE,
    OVERRIDE_SELECTIVE,
    OVERRIDE_SSAO_DEPTH_WRITE,
};

enum ShadowType_t {
    SHADOWS_NONE = 0,
    SHADOWS_SIMPLE,
    SHADOWS_RENDER_TO_TEXTURE,
    SHADOWS_RENDER_TO_TEXTURE_DYNAMIC,	// the shadow is always changing state
    SHADOWS_RENDER_TO_DEPTH_TEXTURE,
    SHADOWS_RENDER_TO_TEXTURE_DYNAMIC_CUSTOM,	// changing, and entity uses custom rendering code for shadow
};

enum ShouldTransmitState_t {
    SHOULDTRANSMIT_START = 0,
    SHOULDTRANSMIT_END
};

enum DataUpdateType_t {
    DATA_UPDATE_CREATED = 0,
    DATA_UPDATE_DATATABLE_CHANGED,
};

struct studiohdr_t {
    int id;
    int version;
    int checksum;
    char name[64];
};

struct DrawModelInfo_t {
    studiohdr_t* m_pStudioHdr;
    PVOID m_pHardwareData; //studiohwdata_t*
    int m_Decals; //StudioDecalHandle_t
    int m_Skin;
    int m_Body;
    int m_HitboxSet;
    void* m_pClientEntity;
    int m_Lod;
    PVOID m_pColorMeshes;
    bool m_bStaticLighting;
    vec3 m_vecAmbientCube[6];
    int m_nLocalLightCount;
    int m_LocalLightDescs[4];
};

struct ModelRenderInfo_t {
    vec3 origin;
    vec3 angles;
    PVOID pRenderable;
    model_t* pModel;
    const matrix3x4* pModelToWorld;
    const matrix3x4* pLightingOffset;
    const vec3* pLightingOrigin;
    int flags;
    int ent_index;
    int skin;
    int body;
    int hitboxset;
    int	instnace;

    ModelRenderInfo_t(void) : pModelToWorld(nullptr), pLightingOffset(nullptr), pLightingOrigin(nullptr) {}
};

struct DrawModelState_t {
    studiohdr_t* m_pStudioHdr;
    PVOID m_pStudioHWData;//studiohwdata_t*
    PVOID m_pRenderable;
    const matrix3x4* m_pModelToWorld;
    void* m_decals;
    int m_drawFlags;
    int m_lod;
};

struct player_info_t {
    uint64_t unknown;
    uint64_t xuid;
    char name[64];
    int	userID;
    char guid[64 + 1];
    uint32_t friendsID;
    char friendsName[64];
    bool fakeplayer;
    bool ishltv;
};

class CGlobalVarsBase {
public:
    float realtime = 0.f;
    int framecount = 0;
    float absoluteframetime = 0.f;
    float curtime = 0.f;
    float frametime = 0.f;
    int maxClients = 0;
    int tickcount = 0;
    float interval_per_tick = 0.f;
    float interpolation_amount = 0.f;
    int simTicksThisFrame = 0;
    int network_protocol = 0;
};

class CKeyValues {
public:	
	enum types_t {
		TYPE_NONE = 0,
		TYPE_STRING,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_PTR,
		TYPE_WSTRING,
		TYPE_COLOR,
		TYPE_UINT64,
		TYPE_NUMTYPES,
	};

	int m_iKeyName;

	char* m_sValue;
	wchar_t* m_wsValue;

	union {
		int m_iValue;
		float m_flValue;
		void* m_pValue;
		unsigned char m_Color[4];
	};

	char m_iDataType;
	char m_bHasEscapeSequences; 
	char m_bEvaluateConditionals;
	char unused[1];

	CKeyValues* m_pPeer; 
	CKeyValues* m_pSub;
	CKeyValues* m_pChain;	
};

class CMaterial {
public:
    /*0*/ virtual void* GetName(void)const = 0;
    /*1*/ virtual void* GetTextureGroupName(void)const = 0;
    /*2*/ virtual void* GetPreviewImageProperties(int*, int*, void*, bool*)const = 0;
    /*3*/ virtual void* GetPreviewImage(unsigned char*, int, int, void*)const = 0;
    /*4*/ virtual void* GetMappingWidth(void) = 0;
    /*5*/ virtual void* GetMappingHeight(void) = 0;
    /*6*/ virtual void* GetNumAnimationFrames(void) = 0;
    /*7*/ virtual void* InMaterialPage(void) = 0;
    /*8*/ virtual void* GetMaterialOffset(float*) = 0;
    /*9*/ virtual void* GetMaterialScale(float*) = 0;
    /*10*/ virtual void* GetMaterialPage(void) = 0;
    /*11*/ virtual void* FindVar(char const*, bool*, bool) = 0;
    /*12*/ virtual void* IncrementReferenceCount(void) = 0;
    /*13*/ virtual void* DecrementReferenceCount(void) = 0;
    /*14*/ virtual void* GetEnumerationID(void)const = 0;
    /*15*/ virtual void* GetLowResColorSample(float, float, float*)const = 0;
    /*16*/ virtual void* RecomputeStateSnapshots(void) = 0;
    /*17*/ virtual void* IsTranslucent(void) = 0;
    /*18*/ virtual void* IsAlphaTested(void) = 0;
    /*19*/ virtual void* IsVertexLit(void) = 0;
    /*20*/ virtual void* GetVertexFormat(void)const = 0;
    /*21*/ virtual void* HasProxy(void)const = 0;
    /*22*/ virtual void* UsesEnvCubemap(void) = 0;
    /*23*/ virtual void* NeedsTangentSpace(void) = 0;
    /*24*/ virtual void* NeedsPowerOfTwoFrameBufferTexture(bool) = 0;
    /*25*/ virtual void* NeedsFullFrameBufferTexture(bool) = 0;
    /*26*/ virtual void* NeedsSoftwareSkinning(void) = 0;
    /*27*/ virtual void* AlphaModulate(float) = 0;
    /*28*/ virtual void* ColorModulate(float, float, float) = 0;
    /*29*/ virtual void* SetMaterialVarFlag(MaterialVarFlags_t, bool) = 0;
    /*30*/ virtual void* GetMaterialVarFlag(MaterialVarFlags_t)const = 0;
    /*31*/ virtual void* GetReflectivity(vec3&) = 0;
    /*32*/ virtual void* GetPropertyFlag(int) = 0;
    /*33*/ virtual void* IsTwoSided(void) = 0;
    /*34*/ virtual void* SetShader(char const*) = 0;
    /*35*/ virtual void* GetNumPasses(void) = 0;
    /*36*/ virtual void* GetTextureMemoryBytes(void) = 0;
    /*37*/ virtual void* Refresh(void) = 0;
    /*38*/ virtual void* NeedsLightmapBlendAlpha(void) = 0;
    /*39*/ virtual void* NeedsSoftwareLighting(void) = 0;
    /*40*/ virtual void* ShaderParamCount(void)const = 0;
    /*41*/ virtual void* GetShaderParams(void) = 0;
    /*42*/ virtual void* IsErrorMaterial(void)const = 0;
    /*43*/ virtual void* SetUseFixedFunctionBakedLighting(bool) = 0;
    /*44*/ virtual void* GetAlphaModulation(void) = 0;
    /*45*/ virtual void* GetColorModulation(float*, float*, float*) = 0;
    /*46*/ virtual void* GetMorphFormat(void)const = 0;
    /*47*/ virtual void* FindVarFast(char const*, unsigned int*) = 0;
    /*48*/ virtual void* SetShaderAndParams(CKeyValues*) = 0;
    /*49*/ virtual void* GetShaderName(void)const = 0;
    /*50*/ virtual void* DeleteIfUnreferenced(void) = 0;
    /*51*/ virtual void* IsSpriteCard(void) = 0;
    /*52*/ virtual void* CallBindProxy(void*) = 0;
    /*53*/ virtual void* CheckProxyReplacement(void*) = 0;
    /*54*/ virtual void* RefreshPreservingMaterialVars(void) = 0;
    /*55*/ virtual void* WasReloadedFromWhitelist(void) = 0;
    /*56*/ virtual void* IsPrecached(void)const = 0;
    /*57*/ virtual void* GetReferenceCount(void)const = 0;
    /*58*/ virtual void* SetEnumerationID(int) = 0;
    /*59*/ virtual void* SetNeedsWhiteLightmap(bool) = 0;
    /*60*/ virtual void* GetNeedsWhiteLightmap(void)const = 0;
    /*61*/ virtual void* Uncache(bool) = 0;
    /*62*/ virtual void* Precache(void) = 0;
    /*63*/ virtual void* PrecacheVars(CKeyValues*, CKeyValues*, void*, int) = 0;
    /*64*/ virtual void* ReloadTextures(void) = 0;
    /*65*/ virtual void* SetMinLightmapPageID(int) = 0;
    /*66*/ virtual void* SetMaxLightmapPageID(int) = 0;
    /*67*/ virtual void* GetMinLightmapPageID(void)const = 0;
    /*68*/ virtual void* GetMaxLightmapPageID(void)const = 0;
    /*69*/ virtual void* GetShader(void)const = 0;
    /*70*/ virtual void* IsPrecachedVars(void)const = 0;
    /*71*/ virtual void* DrawMesh(int) = 0;
    /*72*/ virtual void* GetVertexUsage(void)const = 0;
    /*73*/ virtual void* PerformDebugTrace(void)const = 0;
    /*74*/ virtual void* NoDebugOverride(void)const = 0;
    /*75*/ virtual void* ToggleSuppression(void) = 0;
    /*76*/ virtual void* IsSuppressed(void)const = 0;
    /*77*/ virtual void* ToggleDebugTrace(void) = 0;
    /*78*/ virtual void* UseFog(void)const = 0;
    /*79*/ virtual void* AddMaterialVar(void*) = 0;
    /*80*/ virtual void* GetRenderState(void) = 0;
    /*81*/ virtual void* IsManuallyCreated(void)const = 0;
    /*82*/ virtual void* NeedsFixedFunctionFlashlight(void)const = 0;
    /*83*/ virtual void* IsUsingVertexID(void)const = 0;
    /*84*/ virtual void* MarkAsPreloaded(bool) = 0;
    /*85*/ virtual void* IsPreloaded(void)const = 0;
    /*86*/ virtual void* ArtificialAddRef(void) = 0;
    /*87*/ virtual void* ArtificialRelease(void) = 0;
    /*88*/ virtual void* ReportVarChanged(void*) = 0;
    /*89*/ virtual void* GetChangeID(void)const = 0;
    /*90*/ virtual void* IsTranslucentInternal(float)const = 0;
    /*91*/ virtual void* IsRealTimeVersion(void)const = 0;
    /*92*/ virtual void* ClearContextData(void) = 0;
    /*93*/ virtual void* GetRealTimeVersion(void) = 0;
    /*94*/ virtual void* GetQueueFriendlyVersion(void) = 0;
    /*95*/ virtual void* PrecacheMappingDimensions(void) = 0;
    /*96*/ virtual void* FindRepresentativeTexture(void) = 0;
    /*97*/ virtual void* DecideShouldReloadFromWhitelist(void*) = 0;
    /*98*/ virtual void* ReloadFromWhitelistIfMarked(void) = 0;
};

class IHandleEntity {
public:

};

class IClientUnknown : public IHandleEntity {
public:
    virtual ICollideable* GetCollideable() = 0;
    virtual IClientNetworkable* GetClientNetworkable() = 0;
    virtual IClientRenderable* GetClientRenderable() = 0;
    virtual IClientEntity* GetIClientEntity() = 0;
    virtual C_BaseEntity* GetBaseEntity() = 0;
    virtual IClientThinkable* GetClientThinkable() = 0;
    virtual IClientModelRenderable* GetClientModelRenderable() = 0;
    virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};

class IPVSNotify {
public:
    virtual void OnPVSStatusChanged(bool bInPVS) = 0;
};

class IClientRenderable {
public:
    virtual IClientUnknown* GetIClientUnknown() = 0;
    virtual vec3 const& GetRenderOrigin(void) = 0;
    virtual QAngle const& GetRenderAngles(void) = 0;
    virtual bool ShouldDraw(void) = 0;
    virtual bool IsTransparent(void) = 0;
    virtual bool UsesPowerOfTwoFrameBufferTexture() = 0;
    virtual bool UsesFullFrameBufferTexture() = 0;
    virtual int GetShadowHandle() const = 0;
    virtual int& RenderHandle() = 0;
    virtual const model_t* GetModel() const = 0;
    virtual int DrawModel(int flags) = 0;
    virtual int	GetBody() = 0;
    virtual void ComputeFxBlend() = 0;
    virtual int	 GetFxBlend(void) = 0;
    virtual void GetColorModulation(float* color) = 0;
    virtual bool LODTest() = 0;
    virtual bool SetupBones(matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
    virtual void SetupWeights(const matrix3x4* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
    virtual void DoAnimationEvents(void) = 0;
    virtual IPVSNotify* GetPVSNotifyInterface() = 0;
    virtual void GetRenderBounds(vec3& mins, vec3& maxs) = 0;
    virtual void GetRenderBoundsWorldspace(vec3& mins, vec3& maxs) = 0;
    virtual void GetShadowRenderBounds(vec3& mins, vec3& maxs, ShadowType_t shadowType) = 0;
    virtual bool ShouldReceiveProjectedTextures(int flags) = 0;
    virtual bool GetShadowCastDistance(float* pDist, ShadowType_t shadowType) const = 0;
    virtual bool GetShadowCastDirection(vec3* pDirection, ShadowType_t shadowType) const = 0;
    virtual bool IsShadowDirty() = 0;
    virtual void MarkShadowDirty(bool bDirty) = 0;
    virtual IClientRenderable* GetShadowParent() = 0;
    virtual IClientRenderable* FirstShadowChild() = 0;
    virtual IClientRenderable* NextShadowPeer() = 0;
    virtual ShadowType_t ShadowCastType() = 0;
    virtual void CreateModelInstance() = 0;
    virtual int GetModelInstance() = 0;
    virtual const matrix3x4& RenderableToWorldTransform() = 0;
    virtual int LookupAttachment(const char* pAttachmentName) = 0;
    virtual	bool GetAttachment(int number, vec3& origin, QAngle& angles) = 0;
    virtual bool GetAttachment(int number, matrix3x4& matrix) = 0;
    virtual float* GetRenderClipPlane(void) = 0;
    virtual int	GetSkin() = 0;
    virtual bool IsTwoPass(void) = 0;
    virtual void OnThreadedDrawSetup() = 0;
    virtual bool UsesFlexDelayedWeights() = 0;
    virtual void RecordToolMessage() = 0;
    virtual bool IgnoresZBuffer(void) const = 0;
};

class ClientClass {
public:
    ClientClass(char* pNetworkName, CreateClientClassFn createFn, CreateEventFn createEventFn, RecvTable* pRecvTable) {
        m_pNetworkName = pNetworkName;
        m_pCreateFn = createFn;
        m_pCreateEventFn = createEventFn;
        m_pRecvTable = pRecvTable;

        m_pNext = g_pClientClassHead;
        g_pClientClassHead = this;
    }

    const char* GetName() {
        return m_pNetworkName;
    }

public:
    CreateClientClassFn m_pCreateFn;
    CreateEventFn m_pCreateEventFn;
    char* m_pNetworkName;
    RecvTable* m_pRecvTable;
    ClientClass* m_pNext;
    int m_ClassID;
};

class IClientNetworkable {
public:
    virtual IClientUnknown* GetIClientUnknown() = 0;
    virtual void Release() = 0;
    virtual ClientClass* GetClientClass() = 0;
    virtual void NotifyShouldTransmit(ShouldTransmitState_t state) = 0;
    virtual void OnPreDataChanged(DataUpdateType_t updateType) = 0;
    virtual void OnDataChanged(DataUpdateType_t updateType) = 0;
    virtual void PreDataUpdate(DataUpdateType_t updateType) = 0;
    virtual void PostDataUpdate(DataUpdateType_t updateType) = 0;
    virtual bool IsDormant(void) = 0;
    virtual int	entindex(void) const = 0;
    virtual void ReceiveMessage(int classID, void* msg) = 0;
    virtual void* GetDataTableBasePtr() = 0;
    virtual void SetDestroyedOnRecreateEntities(void) = 0;
};

class IClientThinkable {
public:
    virtual IClientUnknown* get_i_client_unknown() = 0;
    virtual void client_think() = 0;
    virtual client_think_handle_t get_think_handle() = 0;
    virtual void set_think_handle(client_think_handle_t think) = 0;
    virtual void release() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable {
public:
    virtual void Release(void) = 0;
    virtual const vec3& GetAbsOrigin(void) const = 0;
    virtual const QAngle& GetAbsAngles(void) const = 0;
    virtual void* GetMouth(void) = 0;
    virtual bool GetSoundSpatialization(int& info) = 0;
    template <typename T> inline T* as() { return reinterpret_cast<T*>(this); }
};

class CClientEntityList {
public:
    virtual void f1() = 0;
    virtual void f2() = 0;
    virtual void f3() = 0;
    virtual IClientEntity* GetClientEntity(int iIndex) = 0;
    virtual IClientEntity* GetClientEntityFromHandle(DWORD hHandle) = 0;
    virtual int number_of_ents(bool bIncludeNonNetworkable) = 0;
    virtual int GetHighestEntityIndex() = 0;
};

class CMatSystemSurface
{
public:
    /*0*/ virtual void* Connect(void* (*)(char const*, int*)) = 0;
    /*1*/ virtual void* Disconnect(void) = 0;
    /*2*/ virtual void* QueryInterface(char const*) = 0;
    /*3*/ virtual void* Init(void) = 0;
    /*4*/ virtual void* Shutdown(void) = 0;
    /*5*/ virtual void* RunFrame(void) = 0;
    /*6*/ virtual void* GetEmbeddedPanel(void) = 0;
    /*7*/ virtual void* SetEmbeddedPanel(unsigned int) = 0;
    /*8*/ virtual void* PushMakeCurrent(unsigned int, bool) = 0;
    /*9*/ virtual void* PopMakeCurrent(unsigned int) = 0;
    /*10*/ virtual void* DrawSetColor(int, int, int, int) = 0;
    /*11*/ virtual void* DrawSetColor(Vector4D) = 0;
    /*12*/ virtual void* DrawFilledRect(int, int, int, int) = 0;
    /*13*/ virtual void* DrawFilledRectArray(void*, int) = 0;
    /*14*/ virtual void* DrawOutlinedRect(int, int, int, int) = 0;
    /*15*/ virtual void* DrawLine(int, int, int, int) = 0;
    /*16*/ virtual void* DrawPolyLine(int*, int*, int) = 0;
    /*17*/ virtual void* DrawSetTextFont(unsigned int) = 0;
    /*18*/ virtual void* DrawSetTextColor(int, int, int, int) = 0;
    /*19*/ virtual void* DrawSetTextColor(Vector4D) = 0;
    /*20*/ virtual void* DrawSetTextPos(int, int) = 0;
    /*21*/ virtual void* DrawGetTextPos(int&, int&) = 0;
    /*22*/ virtual void* DrawPrintText(wchar_t const*, int, int) = 0;
    /*23*/ virtual void* DrawUnicodeChar(wchar_t, int) = 0;
    /*24*/ virtual void* DrawFlushText(void) = 0;
    /*25*/ virtual void* CreateHTMLWindow(void*, unsigned int) = 0;
    /*26*/ virtual void* PaintHTMLWindow(void*) = 0;
    /*27*/ virtual void* DeleteHTMLWindow(void*) = 0;
    /*28*/ virtual void* DrawGetTextureId(char const*) = 0;
    /*29*/ virtual void* DrawGetTextureFile(int, char*, int) = 0;
    /*30*/ virtual void* DrawSetTextureFile(int, char const*, int, bool) = 0;
    /*31*/ virtual void* DrawSetTextureRGBA(int, unsigned char const*, int, int, int, bool) = 0;
    /*32*/ virtual void* DrawSetTexture(int) = 0;
    /*33*/ virtual void* DrawGetTextureSize(int, int&, int&) = 0;
    /*34*/ virtual void* DrawTexturedRect(int, int, int, int) = 0;
    /*35*/ virtual void* IsTextureIDValid(int) = 0;
    /*36*/ virtual void* DeleteTextureByID(int) = 0;
    /*37*/ virtual void* CreateNewTextureID(bool) = 0;
    /*38*/ virtual void* GetScreenSize(int&, int&) = 0;
    /*39*/ virtual void* SetAsTopMost(unsigned int, bool) = 0;
    /*40*/ virtual void* BringToFront(unsigned int) = 0;
    /*41*/ virtual void* SetForegroundWindow(unsigned int) = 0;
    /*42*/ virtual void* SetPanelVisible(unsigned int, bool) = 0;
    /*43*/ virtual void* SetMinimized(unsigned int, bool) = 0;
    /*44*/ virtual void* IsMinimized(unsigned int) = 0;
    /*45*/ virtual void* FlashWindow(unsigned int, bool) = 0;
    /*46*/ virtual void* SetTitle(unsigned int, wchar_t const*) = 0;
    /*47*/ virtual void* SetAsToolBar(unsigned int, bool) = 0;
    /*48*/ virtual void* CreatePopup(unsigned int, bool, bool, bool, bool, bool) = 0;
    /*49*/ virtual void* SwapBuffers(unsigned int) = 0;
    /*50*/ virtual void* Invalidate(unsigned int) = 0;
    /*51*/ virtual void* SetCursor(unsigned int) = 0;
    /*52*/ virtual void* SetCursorAlwaysVisible(bool) = 0;
    /*53*/ virtual void* IsCursorVisible(void) = 0;
    /*54*/ virtual void* ApplyChanges(void) = 0;
    /*55*/ virtual void* IsWithin(int, int) = 0;
    /*56*/ virtual void* HasFocus(void) = 0;
    /*57*/ virtual void* SupportsFeature(int) = 0;
    /*58*/ virtual void* RestrictPaintToSinglePanel(unsigned int) = 0;
    /*59*/ virtual void* SetModalPanel(unsigned int) = 0;
    /*60*/ virtual void* GetModalPanel(void) = 0;
    /*61*/ virtual void* UnlockCursor(void) = 0;
    /*62*/ virtual void* LockCursor(void) = 0;
    /*63*/ virtual void* SetTranslateExtendedKeys(bool) = 0;
    /*64*/ virtual void* GetTopmostPopup(void) = 0;
    /*65*/ virtual void* SetTopLevelFocus(unsigned int) = 0;
    /*66*/ virtual void* CreateFont__(void) = 0;
    /*67*/ virtual void* SetFontGlyphSet(unsigned int, char const*, int, int, int, int, int, int, int) = 0;
    /*68*/ virtual void* AddCustomFontFile(char const*, char const*) = 0;
    /*69*/ virtual void* GetFontTall(unsigned int) = 0;
    /*70*/ virtual void* GetFontTallRequested(unsigned int) = 0;
    /*71*/ virtual void* GetFontAscent(unsigned int, wchar_t) = 0;
    /*72*/ virtual void* IsFontAdditive(unsigned int) = 0;
    /*73*/ virtual void* GetCharABCwide(unsigned int, int, int&, int&, int&) = 0;
    /*74*/ virtual void* GetCharacterWidth(unsigned int, int) = 0;
    /*75*/ virtual void* GetTextSize(unsigned int, wchar_t const*, int&, int&) = 0;
    /*76*/ virtual void* GetNotifyPanel(void) = 0;
    /*77*/ virtual void* SetNotifyIcon(unsigned int, unsigned int, unsigned int, char const*) = 0;
    /*78*/ virtual void* PlaySound__(char const*) = 0;
    /*79*/ virtual void* GetPopupCount(void) = 0;
    /*80*/ virtual void* GetPopup(int) = 0;
    /*81*/ virtual void* ShouldPaintChildPanel(unsigned int) = 0;
    /*82*/ virtual void* RecreateContext(unsigned int) = 0;
    /*83*/ virtual void* AddPanel(unsigned int) = 0;
    /*84*/ virtual void* ReleasePanel(unsigned int) = 0;
    /*85*/ virtual void* MovePopupToFront(unsigned int) = 0;
    /*86*/ virtual void* MovePopupToBack(unsigned int) = 0;
    /*87*/ virtual void* SolveTraverse(unsigned int, bool) = 0;
    /*88*/ virtual void* PaintTraverse(unsigned int) = 0;
    /*89*/ virtual void* EnableMouseCapture(unsigned int, bool) = 0;
    /*90*/ virtual void* GetWorkspaceBounds(int&, int&, int&, int&) = 0;
    /*91*/ virtual void* GetAbsoluteWindowBounds(int&, int&, int&, int&) = 0;
    /*92*/ virtual void* GetProportionalBase(int&, int&) = 0;
    /*93*/ virtual void* CalculateMouseVisible(void) = 0;
    /*94*/ virtual void* NeedKBInput(void) = 0;
    /*95*/ virtual void* HasCursorPosFunctions(void) = 0;
    /*96*/ virtual void* SurfaceGetCursorPos(int&, int&) = 0;
    /*97*/ virtual void* SurfaceSetCursorPos(int, int) = 0;
    /*98*/ virtual void* DrawTexturedLine(Vertex_t const&, Vertex_t const&) = 0;
    /*99*/ virtual void* DrawOutlinedCircle(int, int, int, int) = 0;
    /*100*/ virtual void* DrawTexturedPolyLine(Vertex_t const*, int) = 0;
    /*101*/ virtual void* DrawTexturedSubRect(int, int, int, int, float, float, float, float) = 0;
    /*102*/ virtual void* DrawTexturedPolygon(int, Vertex_t*, bool) = 0;
    /*103*/ virtual void* GetTitle(unsigned int) = 0;
    /*104*/ virtual void* IsCursorLocked(void)const = 0;
    /*105*/ virtual void* SetWorkspaceInsets(int, int, int, int) = 0;
    /*106*/ virtual void* DrawGetUnicodeCharRenderInfo(wchar_t, int&) = 0;
    /*107*/ virtual void* DrawRenderCharFromInfo(int const&) = 0;
    /*108*/ virtual void* DrawSetAlphaMultiplier(float) = 0;
    /*109*/ virtual void* DrawGetAlphaMultiplier(void) = 0;
    /*110*/ virtual void* SetAllowHTMLJavaScript(bool) = 0;
    /*111*/ virtual void* OnScreenSizeChanged(int, int) = 0;
    /*112*/ virtual void* CreateCursorFromFile(char const*, char const*) = 0;
    /*113*/ virtual void* DrawGetTextureMatInfoFactory(int) = 0;
    /*114*/ virtual void* PaintTraverseEx(unsigned int, bool) = 0;
    /*115*/ virtual void* GetZPos(void)const = 0;
    /*116*/ virtual void* SetPanelForInput(unsigned int) = 0;
    /*117*/ virtual void* DrawFilledRectFastFade(int, int, int, int, int, int, unsigned int, unsigned int, bool) = 0;
    /*118*/ virtual void* DrawFilledRectFade(int, int, int, int, unsigned int, unsigned int, bool) = 0;
    /*119*/ virtual void* DrawSetTextureRGBAEx(int, unsigned char const*, int, int, int) = 0;
    /*120*/ virtual void* DrawSetTextScale(float, float) = 0;
    /*121*/ virtual void* SetBitmapFontGlyphSet(unsigned int, char const*, float, float, int) = 0;
    /*122*/ virtual void* AddBitmapFontFile(char const*) = 0;
    /*123*/ virtual void* SetBitmapFontName(char const*, char const*) = 0;
    /*124*/ virtual void* GetBitmapFontName(char const*) = 0;
    /*125*/ virtual void* ClearTemporaryFontCache(void) = 0;
    /*126*/ virtual void* GetIconImageForFullPath(char const*) = 0;
    /*127*/ virtual void* DrawUnicodeString(wchar_t const*, int) = 0;
    /*128*/ virtual void* PrecacheFontCharacters(unsigned int, wchar_t const*) = 0;
    /*129*/ virtual void* GetResolutionKey(void)const = 0;
    /*130*/ virtual void* GetFontName(unsigned int) = 0;
    /*131*/ virtual void* GetFontFamilyName(unsigned int) = 0;
    /*132*/ virtual void* GetKernedCharWidth(unsigned int, wchar_t, wchar_t, wchar_t, float&, float&) = 0;
    /*133*/ virtual void* ForceScreenSizeOverride(bool, int, int) = 0;
    /*134*/ virtual void* ForceScreenPosOffset(bool, int, int) = 0;
    /*135*/ virtual void* OffsetAbsPos(int&, int&) = 0;
    /*136*/ virtual void* ResetFontCaches(void) = 0;
    /*137*/ virtual void* GetTextureNumFrames(int) = 0;
    /*138*/ virtual void* DrawSetTextureFrame(int, int, unsigned int*) = 0;
    /*139*/ virtual void* IsScreenSizeOverrideActive(void) = 0;
    /*140*/ virtual void* IsScreenPosOverrideActive(void) = 0;
    /*141*/ virtual void* DestroyTextureID(int) = 0;
    /*142*/ virtual void* DrawUpdateRegionTextureRGBA(int, int, int, unsigned char const*, int, int, int) = 0;
    /*143*/ virtual void* BHTMLWindowNeedsPaint(void*) = 0;
    /*144*/ virtual void* GetWebkitHTMLUserAgentString(void) = 0;
    /*145*/ virtual void* Deprecated_AccessChromeHTMLController(void) = 0;
    /*146*/ virtual void* SetFullscreenViewport(int, int, int, int) = 0;
    /*147*/ virtual void* GetFullscreenViewport(int&, int&, int&, int&) = 0;
    /*148*/ virtual void* PushFullscreenViewport(void) = 0;
    /*149*/ virtual void* PopFullscreenViewport(void) = 0;
    /*150*/ virtual void* SetSoftwareCursor(bool) = 0;
    /*151*/ virtual void* PaintSoftwareCursor(void) = 0;
    /*152*/ virtual void* AttachToWindow(void*, bool) = 0;
    /*153*/ virtual void* EnableWindowsMessages(bool) = 0;
    /*154*/ virtual void* Begin3DPaint(int, int, int, int, bool) = 0;
    /*155*/ virtual void* End3DPaint(void) = 0;
    /*156*/ virtual void* DisableClipping(bool) = 0;
    /*157*/ virtual void* GetClippingRect(int&, int&, int&, int&, bool&) = 0;
    /*158*/ virtual void* SetClippingRect(int, int, int, int) = 0;
    /*159*/ virtual void* SetMouseCallbacks(void (*)(int&, int&), void (*)(int, int)) = 0;
    /*160*/ virtual void* InstallPlaySoundFunc(void (*)(char const*)) = 0;
    /*161*/ virtual void* DrawColoredCircle(int, int, float, int, int, int, int) = 0;
    /*162*/ virtual void* DrawColoredText(unsigned int, int, int, int, int, int, int, char const*, ...) = 0;
    /*163*/ virtual void* DrawColoredTextRect(unsigned int, int, int, int, int, int, int, int, int, char const*, ...) = 0;
    /*164*/ virtual void* DrawTextHeight(unsigned int, int, int&, char const*, ...) = 0;
    /*165*/ virtual void* DrawTextLen(unsigned int, char const*, ...) = 0;
    /*166*/ virtual void* DrawPanelIn3DSpace(unsigned int, int const&, int, int, float, float) = 0;
    /*167*/ virtual void* DrawSetTextureMaterial(int, void*) = 0;
    /*168*/ virtual void* HandleInputEvent(int const&) = 0;
    /*169*/ virtual void* Set3DPaintTempRenderTarget(char const*) = 0;
    /*170*/ virtual void* Reset3DPaintTempRenderTarget(void) = 0;
    /*171*/ virtual void* DrawGetTextureMaterial(int) = 0;
    /*172*/ virtual void* GetFullscreenViewportAndRenderTarget(int&, int&, int&, int&, void**) = 0;
    /*173*/ virtual void* SetFullscreenViewportAndRenderTarget(int, int, int, int, void*) = 0;
    /*174*/ virtual void* DrawGetTextureId(void*) = 0;
    /*175*/ virtual void* BeginSkinCompositionPainting(void) = 0;
    /*176*/ virtual void* EndSkinCompositionPainting(void) = 0;
};

class CEngineVGui {
public:
    /*2*/ virtual void* GetPanel(int) = 0;
    /*3*/ virtual void* IsGameUIVisible(void) = 0;
    /*4*/ virtual void* GetWorkshopMap(unsigned int, void*) = 0;
    /*5*/ virtual void* Init(void) = 0;
    /*6*/ virtual void* Connect(void) = 0;
    /*7*/ virtual void* Shutdown(void) = 0;
    /*8*/ virtual void* SetVGUIDirectories(void) = 0;
    /*9*/ virtual void* IsInitialized(void)const = 0;
    /*10*/ virtual void* GetGameUIFactory(void) = 0;
    /*11*/ virtual void* Key_Event(int const&) = 0;
    /*12*/ virtual void* BackwardCompatibility_Paint(void) = 0;
    /*13*/ virtual void* UpdateButtonState(int const&) = 0;
    /*14*/ virtual void* PostInit(void) = 0;
    /*15*/ virtual void Paint(int) = 0;
    /*16*/ virtual void* ActivateGameUI(void) = 0;
    /*17*/ virtual void* HideGameUI(void) = 0;
    /*18*/ virtual void* ShowConsole(void) = 0;
    /*19*/ virtual void* HideConsole(void) = 0;
    /*20*/ virtual void* IsConsoleVisible(void) = 0;
    /*21*/ virtual void* ClearConsole(void) = 0;
    /*22*/ virtual void* HideDebugSystem(void) = 0;
    /*23*/ virtual void* OnLevelLoadingStarted(void) = 0;
    /*24*/ virtual void* OnLevelLoadingFinished(void) = 0;
    /*25*/ virtual void* NotifyOfServerConnect(char const*, int, int, int) = 0;
    /*26*/ virtual void* NotifyOfServerDisconnect(void) = 0;
    /*27*/ virtual void* EnabledProgressBarForNextLoad(void) = 0;
    /*28*/ virtual void* UpdateProgressBar(int) = 0;
    /*29*/ virtual void* UpdateCustomProgressBar(float, wchar_t const*) = 0;
    /*30*/ virtual void* StartCustomProgress(void) = 0;
    /*31*/ virtual void* FinishCustomProgress(void) = 0;
    /*32*/ virtual void* ShowErrorMessage(void) = 0;
    /*33*/ virtual void* ShouldPause(void) = 0;
    /*34*/ virtual void* SetGameDLLPanelsVisible(bool) = 0;
    /*35*/ virtual void* ShowNewGameDialog(int) = 0;
    /*36*/ virtual void* Simulate(void) = 0;
    /*37*/ virtual void* SetNotAllowedToHideGameUI(bool) = 0;
    /*38*/ virtual void* SetNotAllowedToShowGameUI(bool) = 0;
    /*39*/ virtual void* SessionNotification(int, int) = 0;
    /*40*/ virtual void* SystemNotification(int) = 0;
    /*41*/ virtual void* ShowMessageDialog(unsigned int, void*) = 0;
    /*42*/ virtual void* UpdatePlayerInfo(unsigned long long, char const*, int, unsigned char, int, bool) = 0;
    /*43*/ virtual void* SessionSearchResult(int, void*, void*, int) = 0;
    /*44*/ virtual void* OnCreditsFinished(void) = 0;
    /*45*/ virtual void* ValidateStorageDevice(int*) = 0;
    /*46*/ virtual void* ConfirmQuit(void) = 0;
};

class CModelRender {
public:
    /*0*/ virtual void* DrawModel(int, IClientRenderable*, unsigned short, int, model_t const*, vec3 const&, QAngle const&, int, int, int, matrix3x4 const*, matrix3x4 const*) = 0;
    /*1*/ virtual void* ForcedMaterialOverride(CMaterial*, OverrideType_t = OverrideType_t::OVERRIDE_NORMAL) = 0;
    /*2*/ virtual void* SetViewTarget(CStudioHdr const*, int, vec3 const&) = 0;
    /*3*/ virtual void* CreateInstance(IClientRenderable*, void*) = 0;
    /*4*/ virtual void* DestroyInstance(unsigned short) = 0;
    /*5*/ virtual void* SetStaticLighting(unsigned short, void*) = 0;
    /*6*/ virtual void* GetStaticLighting(unsigned short) = 0;
    /*7*/ virtual void* ChangeInstance(unsigned short, IClientRenderable*) = 0;
    /*8*/ virtual void* AddDecal(unsigned short, Ray_t const&, vec3 const&, int, int, bool, int) = 0;
    /*9*/ virtual void* RemoveAllDecals(unsigned short) = 0;
    /*10*/ virtual void* RemoveAllDecalsFromAllModels(void) = 0;
    /*11*/ virtual void* DrawModelShadowSetup(IClientRenderable*, int, int, DrawModelInfo_t*, matrix3x4*) = 0;
    /*12*/ virtual void* DrawModelShadow(IClientRenderable*, DrawModelInfo_t const&, matrix3x4*) = 0;
    /*13*/ virtual void* RecomputeStaticLighting(unsigned short) = 0;
    /*14*/ virtual void* ReleaseAllStaticPropColorData(void) = 0;
    /*15*/ virtual void* RestoreAllStaticPropColorData(void) = 0;
    /*16*/ virtual void* DrawModelEx(ModelRenderInfo_t&) = 0;
    /*17*/ virtual void* DrawModelExStaticProp(ModelRenderInfo_t&) = 0;
    /*18*/ virtual void* DrawModelSetup(ModelRenderInfo_t&, DrawModelState_t*, matrix3x4*, matrix3x4**) = 0;
    /*19*/ virtual void* DrawModelExecute(DrawModelState_t const&, ModelRenderInfo_t const&, matrix3x4*) = 0;
    /*20*/ virtual void* SetupLighting(vec3 const&) = 0;
    /*21*/ virtual void* DrawStaticPropArrayFast(void*, int, bool) = 0;
    /*22*/ virtual void* SuppressEngineLighting(bool) = 0;
    /*23*/ virtual void* SetupColorMeshes(int) = 0;
    /*24*/ virtual void* AddColoredDecal(unsigned short, Ray_t const&, vec3 const&, int, int, Vector4D, bool, int) = 0;
    /*25*/ virtual void* GetMaterialOverride(CMaterial**, OverrideType_t*) = 0;
    /*26*/ virtual void* GetItemName(unsigned int, void const*, char*, unsigned int) = 0;
};

class CMaterialSystem {
public:
    /*2*/ virtual void* QueryInterface(char const*) = 0;
    /*3*/ virtual void* Init(void) = 0;
    /*4*/ virtual void* Shutdown(void) = 0;
    /*5*/ virtual void* Init(char const*, void*, void* (*)(char const*, int*), void* (*)(char const*, int*)) = 0;
    /*6*/ virtual void* SetShaderAPI(char const*) = 0;
    /*7*/ virtual void* SetAdapter(int, int) = 0;
    /*8*/ virtual void* ModInit(void) = 0;
    /*9*/ virtual void* ModShutdown(void) = 0;
    /*10*/ virtual void* SetThreadMode(int, int) = 0;
    /*11*/ virtual void* GetThreadMode(void) = 0;
    /*12*/ virtual void* IsRenderThreadSafe(void) = 0;
    /*13*/ virtual void* ExecuteQueued(void) = 0;
    /*14*/ virtual void* GetHardwareConfig(char const*, int*) = 0;
    /*15*/ virtual void* UpdateConfig(bool) = 0;
    /*16*/ virtual void* OverrideConfig(int const&, bool) = 0;
    /*17*/ virtual void* GetCurrentConfigForVideoCard(void)const = 0;
    /*18*/ virtual void* GetRecommendedConfigurationInfo(int, CKeyValues*) = 0;
    /*19*/ virtual void* GetDisplayAdapterCount(void)const = 0;
    /*20*/ virtual void* GetCurrentAdapter(void)const = 0;
    /*21*/ virtual void* GetDisplayAdapterInfo(int, int&)const = 0;
    /*22*/ virtual void* GetModeCount(int)const = 0;
    /*23*/ virtual void* GetModeInfo(int, int, int&)const = 0;
    /*24*/ virtual void* AddModeChangeCallBack(void (*)(void)) = 0;
    /*25*/ virtual void* GetDisplayMode(int&)const = 0;
    /*26*/ virtual void* SetMode(void*, int const&) = 0;
    /*27*/ virtual void* SupportsMSAAMode(int) = 0;
    /*28*/ virtual void* GetVideoCardIdentifier(void)const = 0;
    /*29*/ virtual void* SpewDriverInfo(void)const = 0;
    /*30*/ virtual void* GetDXLevelDefaults(unsigned int&, unsigned int&) = 0;
    /*31*/ virtual void* GetBackBufferDimensions(int&, int&)const = 0;
    /*32*/ virtual void* GetBackBufferFormat(void)const = 0;
    /*33*/ virtual void* SupportsHDRMode(int) = 0;
    /*34*/ virtual void* AddView(void*) = 0;
    /*35*/ virtual void* RemoveView(void*) = 0;
    /*36*/ virtual void* SetView(void*) = 0;
    /*37*/ virtual void* BeginFrame(float) = 0;
    /*38*/ virtual void* EndFrame(void) = 0;
    /*39*/ virtual void* Flush(bool) = 0;
    /*40*/ virtual void* SwapBuffers(void) = 0;
    /*41*/ virtual void* EvictManagedResources(void) = 0;
    /*42*/ virtual void* ReleaseResources(void) = 0;
    /*43*/ virtual void* ReacquireResources(void) = 0;
    /*44*/ virtual void* AddReleaseFunc(void (*)(void)) = 0;
    /*45*/ virtual void* RemoveReleaseFunc(void (*)(void)) = 0;
    /*46*/ virtual void* AddRestoreFunc(void (*)(int)) = 0;
    /*47*/ virtual void* RemoveRestoreFunc(void (*)(int)) = 0;
    /*48*/ virtual void* ResetTempHWMemory(bool) = 0;
    /*49*/ virtual void* HandleDeviceLost(void) = 0;
    /*50*/ virtual void* ShaderCount(void)const = 0;
    /*51*/ virtual void* GetShaders(int, int, void**)const = 0;
    /*52*/ virtual void* ShaderFlagCount(void)const = 0;
    /*53*/ virtual void* ShaderFlagName(int)const = 0;
    /*54*/ virtual void* GetShaderFallback(char const*, char*, int) = 0;
    /*55*/ virtual void* GetMaterialProxyFactory(void) = 0;
    /*56*/ virtual void* SetMaterialProxyFactory(void*) = 0;
    /*57*/ virtual void* EnableEditorMaterials(void) = 0;
    /*58*/ virtual void* SetInStubMode(bool) = 0;
    /*59*/ virtual void* DebugPrintUsedMaterials(char const*, bool) = 0;
    /*60*/ virtual void* DebugPrintUsedTextures(void) = 0;
    /*61*/ virtual void* ToggleSuppressMaterial(char const*) = 0;
    /*62*/ virtual void* ToggleDebugMaterial(char const*) = 0;
    /*63*/ virtual void* UsingFastClipping(void) = 0;
    /*64*/ virtual void* StencilBufferBits(void) = 0;
    /*65*/ virtual void* SuspendTextureStreaming(void) = 0;
    /*66*/ virtual void* ResumeTextureStreaming(void) = 0;
    /*67*/ virtual void* UncacheAllMaterials(void) = 0;
    /*68*/ virtual void* UncacheUnusedMaterials(bool) = 0;
    /*69*/ virtual void* CacheUsedMaterials(void) = 0;
    /*70*/ virtual void* ReloadTextures(void) = 0;
    /*71*/ virtual void* ReloadMaterials(char const*) = 0;
    /*72*/ virtual CMaterial* CreateMaterial(char const*, CKeyValues*) = 0;
    /*73*/ virtual CMaterial* FindMaterial(char const*, char const*, bool = true, char const* = 0) = 0;
    /*74*/ virtual void* IsMaterialLoaded(char const*) = 0;
    /*75*/ virtual MaterialHandle_t FirstMaterial(void)const = 0;
    /*76*/ virtual MaterialHandle_t NextMaterial(void)const = 0;
    /*77*/ virtual void* InvalidMaterial(void)const = 0;
    /*78*/ virtual CMaterial* GetMaterial(MaterialHandle_t)const = 0;
    /*79*/ virtual void* GetNumMaterials(void)const = 0;
    /*80*/ virtual void* SetAsyncTextureLoadCache(void*) = 0;
    /*81*/ virtual void* FindTexture(char const*, char const*, bool, int) = 0;
    /*82*/ virtual void* IsTextureLoaded(char const*)const = 0;
    /*83*/ virtual void* CreateProceduralTexture(char const*, char const*, int, int, int, int) = 0;
    /*84*/ virtual void* BeginRenderTargetAllocation(void) = 0;
    /*85*/ virtual void* EndRenderTargetAllocation(void) = 0;
    /*86*/ virtual void* CreateRenderTargetTexture(int, int, int, int, int) = 0;
    /*87*/ virtual void* CreateNamedRenderTargetTextureEx(char const*, int, int, int, int, int, unsigned int, unsigned int) = 0;
    /*88*/ virtual void* CreateNamedRenderTargetTexture(char const*, int, int, int, int, int, bool, bool) = 0;
    /*89*/ virtual void* CreateNamedRenderTargetTextureEx2(char const*, int, int, int, int, int, unsigned int, unsigned int) = 0;
    /*90*/ virtual void* BeginLightmapAllocation(void) = 0;
    /*91*/ virtual void* EndLightmapAllocation(void) = 0;
    /*92*/ virtual void* AllocateLightmap(int, int, int*, void*) = 0;
    /*93*/ virtual void* AllocateWhiteLightmap(void*) = 0;
    /*94*/ virtual void* UpdateLightmap(int, int*, int*, float*, float*, float*, float*) = 0;
    /*95*/ virtual void* GetNumSortIDs(void) = 0;
    /*96*/ virtual void* GetSortInfo(void*) = 0;
    /*97*/ virtual void* GetLightmapPageSize(int, int*, int*)const = 0;
    /*98*/ virtual void* ResetMaterialLightmapPageInfo(void) = 0;
    /*99*/ virtual void* ClearBuffers(bool, bool, bool) = 0;
    /*100*/ virtual void* GetRenderContext(void) = 0;
    /*101*/ virtual void* SupportsShadowDepthTextures(void) = 0;
    /*102*/ virtual void* BeginUpdateLightmaps(void) = 0;
    /*103*/ virtual void* EndUpdateLightmaps(void) = 0;
    /*104*/ virtual void* Lock(void) = 0;
    /*105*/ virtual void* Unlock(void*) = 0;
    /*106*/ virtual void* GetShadowDepthTextureFormat(void) = 0;
    /*107*/ virtual void* SupportsFetch4(void) = 0;
    /*108*/ virtual void* CreateRenderContext(int) = 0;
    /*109*/ virtual void* SetRenderContext(void*) = 0;
    /*110*/ virtual void* SupportsCSAAMode(int, int) = 0;
    /*111*/ virtual void* RemoveModeChangeCallBack(void (*)(void)) = 0;
    /*112*/ virtual void* FindProceduralMaterial(char const*, char const*, CKeyValues*) = 0;
    /*113*/ virtual void* GetNullTextureFormat(void) = 0;
    /*114*/ virtual void* AddTextureAlias(char const*, char const*) = 0;
    /*115*/ virtual void* RemoveTextureAlias(char const*) = 0;
    /*116*/ virtual void* AllocateDynamicLightmap(int*, int*, int) = 0;
    /*117*/ virtual void* SetExcludedTextures(char const*) = 0;
    /*118*/ virtual void* UpdateExcludedTextures(void) = 0;
    /*119*/ virtual void* IsInFrame(void)const = 0;
    /*120*/ virtual void* CompactMemory(void) = 0;
    /*121*/ virtual void* ReloadFilesInList(void*) = 0;
    /*122*/ virtual void* AllowThreading(bool, int) = 0;
    /*123*/ virtual CMaterial* FindMaterialEx(char const*, char const*, void*, bool, char const*) = 0;
    /*124*/ virtual void* DoStartupShaderPreloading(void) = 0;
    /*125*/ virtual void* SetRenderTargetFrameBufferSizeOverrides(int, int) = 0;
    /*126*/ virtual void* GetRenderTargetFrameBufferDimensions(int&, int&) = 0;
    /*127*/ virtual void* GetDisplayDeviceName(void)const = 0;
    /*128*/ virtual void* CreateTextureFromBits(int, int, int, int, int, unsigned char*) = 0;
    /*129*/ virtual void* OverrideRenderTargetAllocation(bool) = 0;
    /*130*/ virtual void* NewTextureCompositor(int, int, char const*, int, unsigned long long, CKeyValues*, unsigned int) = 0;
    /*131*/ virtual void* AsyncFindTexture(char const*, char const*, void*, void*, bool, int) = 0;
    /*132*/ virtual void* CreateNamedTextureFromBitsEx(char const*, char const*, int, int, int, int, int, unsigned char*, int) = 0;
    /*133*/ virtual void* AddTextureCompositorTemplate(char const*, CKeyValues*, int) = 0;
    /*134*/ virtual void* VerifyTextureCompositorTemplates(void) = 0;
    /*135*/ virtual void* GetRenderBackend(void)const = 0;
    /*136*/ virtual void* GetCurrentMaterial(void) = 0;
    /*137*/ virtual void* GetLightmapPage(void) = 0;
    /*138*/ virtual void* GetLightmapWidth(int)const = 0;
    /*139*/ virtual void* GetLightmapHeight(int)const = 0;
    /*140*/ virtual void* GetLocalCubemap(void) = 0;
    /*141*/ virtual void* ForceDepthFuncEquals(bool) = 0;
    /*142*/ virtual void* GetHeightClipMode(void) = 0;
    /*143*/ virtual void* AddMaterialToMaterialList(void*) = 0;
    /*144*/ virtual void* RemoveMaterial(void*) = 0;
    /*145*/ virtual void* RemoveMaterialSubRect(void*) = 0;
    /*146*/ virtual void* InFlashlightMode(void)const = 0;
    /*147*/ virtual void* CanUseEditorMaterials(void)const = 0;
    /*148*/ virtual void* GetForcedTextureLoadPathID(void) = 0;
    /*149*/ virtual void* GetRenderCallQueue(void) = 0;
    /*150*/ virtual void* UnbindMaterial(CMaterial*) = 0;
    /*151*/ virtual void* GetRenderThreadId(void)const = 0;
    /*152*/ virtual void* DetermineProxyReplacements(CMaterial*, CKeyValues*) = 0;
    /*153*/ virtual void* GetConfig(void) = 0;
    /*154*/ virtual void* NoteAnisotropicLevel(int) = 0;
    /*155*/ virtual void* ReleaseShaderObjects(void) = 0;
    /*156*/ virtual void* RestoreShaderObjects(void* (*)(char const*, int*), int) = 0;
    /*157*/ virtual void* InEditorMode(void)const = 0;
    /*158*/ virtual void* IsInStubMode(void) = 0;
    /*159*/ virtual void* ImageFormatInfo(int)const = 0;
    /*160*/ virtual void* GetMemRequired(int, int, int, int, bool) = 0;
    /*161*/ virtual void* ConvertImageFormat(unsigned char*, int, unsigned char*, int, int, int, int, int) = 0;
    /*162*/ virtual void* OnDrawMesh(void*, int, int) = 0;
    /*163*/ virtual void* OnDrawMesh(void*, void*, int) = 0;
    /*164*/ virtual void* OnSetFlexMesh(void*, void*, int) = 0;
    /*165*/ virtual void* OnSetColorMesh(void*, void*, int) = 0;
    /*166*/ virtual void* OnSetPrimitiveType(void*, int) = 0;
    /*167*/ virtual void* SyncMatrices(void) = 0;
    /*168*/ virtual void* SyncMatrix(int) = 0;
    /*169*/ virtual void* OnFlushBufferedPrimitives(void) = 0;
    /*170*/ virtual void* OnThreadEvent(unsigned int) = 0;
    /*171*/ virtual void* BindStandardTexture(int, int) = 0;
    /*172*/ virtual void* BindStandardVertexTexture(int, int) = 0;
    /*173*/ virtual void* GetLightmapDimensions(int*, int*) = 0;
    /*174*/ virtual void* GetStandardTextureDimensions(int*, int*, int) = 0;
    /*175*/ virtual void* GetBoundMorphFormat(void) = 0;
    /*176*/ virtual void* GetRenderTargetEx(int) = 0;
    /*177*/ virtual void* DrawClearBufferQuad(unsigned char, unsigned char, unsigned char, unsigned char, bool, bool, bool) = 0;
    /*178*/ virtual void* MaxHWMorphBatchCount(void)const = 0;
    /*179*/ virtual void* GetCurrentColorCorrection(void*) = 0;
};

class CModelInfoClient {
public:
    /*0*/ virtual void* __destructor__() = 0;
    /*1*/ virtual void* GetModel(int) = 0;
    /*2*/ virtual void* GetModelIndex(char const*)const = 0;
    /*3*/ virtual const char* GetModelName(model_t const*)const = 0;
    /*4*/ virtual void* GetVCollide(model_t const*) = 0;
    /*5*/ virtual void* GetVCollide(int) = 0;
    /*6*/ virtual void* GetModelBounds(model_t const*, vec3&, vec3&)const = 0;
    /*7*/ virtual void* GetModelRenderBounds(model_t const*, vec3&, vec3&)const = 0;
    /*8*/ virtual void* GetModelFrameCount(model_t const*)const = 0;
    /*9*/ virtual void* GetModelType(model_t const*)const = 0;
    /*10*/ virtual void* GetModelExtraData(model_t const*) = 0;
    /*11*/ virtual void* ModelHasMaterialProxy(model_t const*)const = 0;
    /*12*/ virtual void* IsTranslucent(model_t const*)const = 0;
    /*13*/ virtual void* IsTranslucentTwoPass(model_t const*)const = 0;
    /*14*/ virtual void* RecomputeTranslucency(model_t const*, int, int, void*, float) = 0;
    /*15*/ virtual void* GetModelMaterialCount(model_t const*)const = 0;
    /*16*/ virtual void* GetModelMaterials(model_t const*, int, CMaterial**) = 0;
    /*17*/ virtual void* IsModelVertexLit(model_t const*)const = 0;
    /*18*/ virtual void* GetModelKeyValueText(model_t const*) = 0;
    /*19*/ virtual void* GetModelKeyValue(model_t const*, void*) = 0;
    /*20*/ virtual void* GetModelRadius(model_t const*) = 0;
    /*21*/ virtual void* FindModel(studiohdr_t const*, void**, char const*)const = 0;
    /*22*/ virtual void* FindModel(void*)const = 0;
    /*23*/ virtual void* GetVirtualModel(studiohdr_t const*)const = 0;
    /*24*/ virtual void* GetAnimBlock(studiohdr_t const*, int)const = 0;
    /*25*/ virtual void* GetModelMaterialColorAndLighting(model_t const*, vec3 const&, QAngle const&, void*, vec3&, vec3&) = 0;
    /*26*/ virtual void* GetIlluminationPoint(model_t const*, IClientRenderable*, vec3 const&, QAngle const&, vec3*) = 0;
    /*27*/ virtual void* GetModelContents(int) = 0;
    /*28*/ virtual void* GetStudiomodel(model_t const*) = 0;
    /*29*/ virtual void* GetModelSpriteWidth(model_t const*)const = 0;
    /*30*/ virtual void* GetModelSpriteHeight(model_t const*)const = 0;
    /*31*/ virtual void* SetLevelScreenFadeRange(float, float) = 0;
    /*32*/ virtual void* GetLevelScreenFadeRange(float*, float*)const = 0;
    /*33*/ virtual void* SetViewScreenFadeRange(float, float) = 0;
    /*34*/ virtual void* ComputeLevelScreenFade(vec3 const&, float, float)const = 0;
    /*35*/ virtual void* ComputeViewScreenFade(vec3 const&, float, float)const = 0;
    /*36*/ virtual void* GetAutoplayList(studiohdr_t const*, unsigned short**)const = 0;
    /*37*/ virtual void* GetCollideForVirtualTerrain(int) = 0;
    /*38*/ virtual void* IsUsingFBTexture(model_t const*, int, int, void*)const = 0;
    /*39*/ virtual void* FindOrLoadModel(char const*) = 0;
    /*40*/ virtual void* InitDynamicModels(void) = 0;
    /*41*/ virtual void* ShutdownDynamicModels(void) = 0;
    /*42*/ virtual void* AddDynamicModel(char const*, int) = 0;
    /*43*/ virtual void* ReferenceModel(int) = 0;
    /*44*/ virtual void* UnreferenceModel(int) = 0;
    /*45*/ virtual void* CleanupDynamicModels(bool) = 0;
    /*46*/ virtual void* GetCacheHandle(model_t const*)const = 0;
    /*47*/ virtual void* GetBrushModelPlaneCount(model_t const*)const = 0;
    /*48*/ virtual void* GetBrushModelPlane(model_t const*, int, int&, vec3*)const = 0;
    /*49*/ virtual void* GetSurfacepropsForVirtualTerrain(int) = 0;
    /*50*/ virtual void* OnLevelChange(void) = 0;
    /*51*/ virtual void* GetModelClientSideIndex(char const*)const = 0;
    /*52*/ virtual void* RegisterDynamicModel(char const*, bool) = 0;
    /*53*/ virtual void* IsDynamicModelLoading(int) = 0;
    /*54*/ virtual void* AddRefDynamicModel(int) = 0;
    /*55*/ virtual void* ReleaseDynamicModel(int) = 0;
    /*56*/ virtual void* RegisterModelLoadCallback(int, void*, bool) = 0;
    /*57*/ virtual void* UnregisterModelLoadCallback(int, void*) = 0;
    /*58*/ virtual void* OnDynamicModelsStringTableChange(int, char const*, void const*) = 0;
    /*59*/ virtual void* GetDynamicModelStringTable(void)const = 0;
    /*60*/ virtual void* LookupPrecachedModelIndex(char const*)const = 0;
};

class CStudioRenderContext {
public:
    /*0*/ virtual void* Connect(void* (*)(char const*, int*)) = 0;
    /*1*/ virtual void* Disconnect(void) = 0;
    /*2*/ virtual void* QueryInterface(char const*) = 0;
    /*3*/ virtual void* Init(void) = 0;
    /*4*/ virtual void* Shutdown(void) = 0;
    /*5*/ virtual void* BeginFrame(void) = 0;
    /*6*/ virtual void* EndFrame(void) = 0;
    /*7*/ virtual void* Mat_Stub(CMaterialSystem*) = 0;
    /*8*/ virtual void* UpdateConfig(int const&) = 0;
    /*9*/ virtual void* GetCurrentConfig(int&) = 0;
    /*10*/ virtual void* LoadModel(studiohdr_t*, void*, void*) = 0;
    /*11*/ virtual void* UnloadModel(void*) = 0;
    /*12*/ virtual void* RefreshStudioHdr(studiohdr_t*, void*) = 0;
    /*13*/ virtual void* SetEyeViewTarget(studiohdr_t const*, int, vec3 const&) = 0;
    /*14*/ virtual void* GetNumAmbientLightSamples(void) = 0;
    /*15*/ virtual void* GetAmbientLightDirections(void) = 0;
    /*16*/ virtual void* SetAmbientLightColors(Vector4D const*) = 0;
    /*17*/ virtual void* SetAmbientLightColors(vec3 const*) = 0;
    /*18*/ virtual void* SetLocalLights(int, int const*) = 0;
    /*19*/ virtual void* SetViewState(vec3 const&, vec3 const&, vec3 const&, vec3 const&) = 0;
    /*20*/ virtual void* LockFlexWeights(int, float**, float**) = 0;
    /*21*/ virtual void* UnlockFlexWeights(void) = 0;
    /*22*/ virtual void* LockBoneMatrices(int) = 0;
    /*23*/ virtual void* UnlockBoneMatrices(void) = 0;
    /*24*/ virtual void* GetNumLODs(int const&)const = 0;
    /*25*/ virtual void* GetLODSwitchValue(int const&, int)const = 0;
    /*26*/ virtual void* SetLODSwitchValue(int&, int, float) = 0;
    /*27*/ virtual void* SetColorModulation(float const*) = 0;
    /*28*/ virtual void* SetAlphaModulation(float) = 0;
    /*29*/ virtual void* DrawModel(void*, DrawModelInfo_t const&, matrix3x4*, float*, float*, vec3 const&, int) = 0;
    /*30*/ virtual void* DrawModelStaticProp(DrawModelInfo_t const&, matrix3x4 const&, int) = 0;
    /*31*/ virtual void* DrawStaticPropDecals(DrawModelInfo_t const&, matrix3x4 const&) = 0;
    /*32*/ virtual void* DrawStaticPropShadows(DrawModelInfo_t const&, matrix3x4 const&, int) = 0;
    /*33*/ virtual void* ForcedMaterialOverride(CMaterial*, OverrideType_t) = 0;
    /*34*/ virtual void* CreateDecalList(void*) = 0;
    /*35*/ virtual void* DestroyDecalList(void*) = 0;
    /*36*/ virtual void* AddDecal(void*, studiohdr_t*, matrix3x4*, void*, vec3 const&, CMaterial*, float, int, bool, int) = 0;
    /*37*/ virtual void* ComputeLighting(vec3 const*, int, void*, vec3 const&, vec3 const&, vec3&) = 0;
    /*38*/ virtual void* ComputeLightingConstDirectional(vec3 const*, int, void*, vec3 const&, vec3 const&, vec3&, float) = 0;
    /*39*/ virtual void* AddShadow(CMaterial*, void*, void*, VMatrix*, void*) = 0;
    /*40*/ virtual void* ClearAllShadows(void) = 0;
    /*41*/ virtual void* ComputeModelLod(void*, float, float*) = 0;
    /*42*/ virtual void* GetPerfStats(void*, DrawModelInfo_t const&, void*)const = 0;
    /*43*/ virtual void* GetTriangles(DrawModelInfo_t const&, matrix3x4*, int&) = 0;
    /*44*/ virtual void* GetMaterialList(studiohdr_t*, int, CMaterial**) = 0;
    /*45*/ virtual void* GetMaterialListFromBodyAndSkin(unsigned short, int, int, int, CMaterial**) = 0;
    /*46*/ virtual void* DrawModelArray(DrawModelInfo_t const&, int, void*, int, int) = 0;
    /*47*/ virtual void* GetMaterialOverride(CMaterial**, void*) = 0;
};

class CViewSetup {
public:
    int x{};
    int m_nUnscaledX{};
    int y{};
    int m_nUnscaledY{};
    int width{};
    int m_nUnscaledWidth{};
    int height{};
    int m_eStereoEye{};
    int m_nUnscaledHeight{};
    bool m_bOrtho{};
    float m_OrthoLeft{};
    float m_OrthoTop{};
    float m_OrthoRight{};
    float m_OrthoBottom{};
    float fov{};
    float fovViewmodel{};
    vec3 origin{};
    vec3 angles{};
    float zNear{};
    float zFar{};
    float zNearViewmodel{};
    float zFarViewmodel{};
    bool m_bRenderToSubrectOfLargerScreen{};
    float m_flAspectRatio{};
    bool m_bOffCenter{};
    float m_flOffCenterTop{};
    float m_flOffCenterBottom{};
    float m_flOffCenterLeft{};
    float m_flOffCenterRight{};
    bool m_bDoBloomAndToneMapping{};
    bool m_bCacheFullSceneState{};
    bool m_bViewToProjectionOverride{};
    VMatrix m_ViewToProjection{};
};

class CVRenderView {
public:
    /*0*/ virtual void* DrawBrushModel(IClientEntity*, model_t*, vec3 const&, QAngle const&, bool) = 0;
    /*1*/ virtual void* DrawIdentityBrushModel(IWorldRenderList*, model_t*) = 0;
    /*2*/ virtual void* TouchLight(dlight_t*) = 0;
    /*3*/ virtual void* Draw3DDebugOverlays(void) = 0;
    /*4*/ virtual void* SetBlend(float) = 0;
    /*5*/ virtual void* GetBlend(void) = 0;
    /*6*/ virtual void* SetColorModulation(float const*) = 0;
    /*7*/ virtual void* GetColorModulation(float*) = 0;
    /*8*/ virtual void* SceneBegin(void) = 0;
    /*9*/ virtual void SceneEnd(void) = 0;
    /*10*/ virtual void* GetVisibleFogVolume(vec3 const&, VisibleFogVolumeInfo_t*) = 0;
    /*11*/ virtual void* CreateWorldList(void) = 0;
    /*12*/ virtual void* BuildWorldLists(IWorldRenderList*, WorldListInfo_t*, int, VisOverrideData_t const*, bool, float*) = 0;
    /*13*/ virtual void* DrawWorldLists(IWorldRenderList*, unsigned long, float) = 0;
    /*14*/ virtual void* DrawTopView(bool) = 0;
    /*15*/ virtual void* TopViewBounds(vec2 const&, vec2 const&) = 0;
    /*16*/ virtual void* DrawLights(void) = 0;
    /*17*/ virtual void* DrawMaskEntities(void) = 0;
    /*18*/ virtual void* DrawTranslucentSurfaces(IWorldRenderList*, int, unsigned long, bool) = 0;
    /*19*/ virtual void* DrawLineFile(void) = 0;
    /*20*/ virtual void* DrawLightmaps(IWorldRenderList*, int) = 0;
    /*21*/ virtual void* ViewSetupVis(bool, int, vec3 const*) = 0;
    /*22*/ virtual void* AreAnyLeavesVisible(int*, int) = 0;
    /*23*/ virtual void* VguiPaint(void) = 0;
    /*24*/ virtual void* ViewDrawFade(unsigned char*, CMaterial*) = 0;
    /*25*/ virtual void* OLD_SetProjectionMatrix(float, float, float) = 0;
    /*26*/ virtual void* GetLightAtPoint(vec3&) = 0;
    /*27*/ virtual void* GetViewEntity(void) = 0;
    /*28*/ virtual void* GetFieldOfView(void) = 0;
    /*29*/ virtual void* GetAreaBits(void) = 0;
    /*30*/ virtual void* SetFogVolumeState(int, bool) = 0;
    /*31*/ virtual void* InstallBrushSurfaceRenderer(IBrushRenderer*) = 0;
    /*32*/ virtual void* DrawBrushModelShadow(IClientRenderable*) = 0;
    /*33*/ virtual void* LeafContainsTranslucentSurfaces(IWorldRenderList*, int, unsigned long) = 0;
    /*34*/ virtual void* DoesBoxIntersectWaterVolume(vec3 const&, vec3 const&, int) = 0;
    /*35*/ virtual void* SetAreaState(unsigned char*, unsigned char*) = 0;
    /*36*/ virtual void* VGui_Paint(int) = 0;
    /*37*/ virtual void* Push3DView(CViewSetup const&, int, ITexture*, VPlane*) = 0;
    /*38*/ virtual void* Push2DView(CViewSetup const&, int, ITexture*, VPlane*) = 0;
    /*39*/ virtual void* PopView(VPlane*) = 0;
    /*40*/ virtual void* SetMainView(vec3 const&, QAngle const&) = 0;
    /*41*/ virtual void* ViewSetupVisEx(bool, int, vec3 const*, unsigned int&) = 0;
    /*42*/ virtual void* OverrideViewFrustum(VPlane*) = 0;
    /*43*/ virtual void* DrawBrushModelShadowDepth(IClientEntity*, model_t*, vec3 const&, QAngle const&, int) = 0;
    /*44*/ virtual void* UpdateBrushModelLightmap(model_t*, IClientRenderable*) = 0;
    /*45*/ virtual void* BeginUpdateLightmaps(void) = 0;
    /*46*/ virtual void* EndUpdateLightmaps(void) = 0;
    /*47*/ virtual void* OLD_SetOffCenterProjectionMatrix(float, float, float, float, float, float, float, float) = 0;
    /*48*/ virtual void* OLD_SetProjectionMatrixOrtho(float, float, float, float, float, float) = 0;
    /*49*/ virtual void* Push3DView(CViewSetup const&, int, ITexture*, VPlane*, ITexture*) = 0;
    /*50*/ virtual void* GetMatricesForView(CViewSetup const&, VMatrix*, VMatrix*, VMatrix*, VMatrix*) = 0;
    /*51*/ virtual void* DrawBrushModelEx(IClientEntity*, model_t*, vec3 const&, QAngle const&, int) = 0;
    /*52*/ virtual void* EnumerateLeaf(int, int) = 0;
};

class IVEngineClient {
public:
    virtual int GetIntersectingSurfaces(const model_t* model, const vec3& vCenter, const float radius, const bool bOnlyVisibleSurfaces,	SurfInfo* pInfos, const int nMaxInfos) = 0;
    virtual vec3 GetLightForPoint(const vec3& pos, bool bClamp) = 0;
    virtual CMaterial* TraceLineMaterialAndLighting(const vec3& start, const vec3& end, vec3& diffuseLightColor, vec3& baseColor) = 0;
    virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0;
    virtual bool CopyFile__(const char* source, const char* destination) = 0;
    virtual void GetScreenSize(int& width, int& height) = 0;
    virtual void ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
    virtual void ClientCmd(const char* szCmdString) = 0;
    virtual bool GetPlayerInfo(int ent_num, player_info_t* pinfo) = 0;
    virtual int GetPlayerForUserID(int userID) = 0;
    virtual client_textmessage_t* TextMessageGet(const char* pName) = 0;
    virtual bool Con_IsVisible(void) = 0;
    virtual int GetLocalPlayer(void) = 0;
    virtual const model_t* LoadModel(const char* pName, bool bProp = false) = 0;
    virtual float GetLastTimeStamp(void) = 0;
    virtual CSentence* GetSentence(CAudioSource* pAudioSource) = 0;
    virtual float GetSentenceLength(CAudioSource* pAudioSource) = 0;
    virtual bool IsStreaming(CAudioSource* pAudioSource) const = 0;
    virtual void GetViewAngles(QAngle& va) = 0;
    virtual void SetViewAngles(QAngle& va) = 0;
    virtual int	GetMaxClients(void) = 0;
    virtual	const char* Key_LookupBinding(const char* pBinding) = 0;
    virtual const char* Key_BindingForKey(int& code) = 0;
    virtual void StartKeyTrapMode(void) = 0;
    virtual bool CheckDoneKeyTrapping(int& code) = 0;
    virtual bool IsInGame(void) = 0;
    virtual bool IsConnected(void) = 0;
    virtual bool IsDrawingLoadingImage(void) = 0;
    virtual void HideLoadingPlaque(void) = 0;
    virtual void Con_NPrintf(int pos, const char* fmt, ...) = 0;
    virtual void Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;
    virtual int	 IsBoxVisible(const vec3& mins, const vec3& maxs) = 0;
    virtual int	 IsBoxInViewCluster(const vec3& mins, const vec3& maxs) = 0;
    virtual bool CullBox(const vec3& mins, const vec3& maxs) = 0;
    virtual void Sound_ExtraUpdate(void) = 0;
    virtual const char* GetGameDirectory(void) = 0;
    virtual const VMatrix& WorldToScreenMatrix() = 0;
    virtual const VMatrix& WorldToViewMatrix() = 0;
    virtual int GameLumpVersion(int lumpId) const = 0;
    virtual int GameLumpSize(int lumpId) const = 0;
    virtual bool LoadGameLump(int lumpId, void* pBuffer, int size) = 0;
    virtual int	 LevelLeafCount() const = 0;
    virtual ISpatialQuery* GetBSPTreeQuery() = 0;
    virtual void LinearToGamma(float* linear, float* gamma) = 0;
    virtual float LightStyleValue(int style) = 0;
    virtual void ComputeDynamicLighting(const vec3& pt, const vec3* pNormal, vec3& color) = 0;
    virtual void GetAmbientLightColor(vec3& color) = 0;
    virtual int	 GetDXSupportLevel() = 0;
    virtual bool SupportsHDR() = 0;
    virtual void Mat_Stub(CMaterialSystem* pMatSys) = 0;
    virtual void GetChapterName(char* pchBuff, int iMaxLength) = 0;
    virtual char const* GetLevelName(void) = 0;
    virtual char const* GetLevelNameShort(void) = 0;
    virtual struct IVoiceTweak_s* GetVoiceTweakAPI(void) = 0;
    virtual void EngineStats_BeginFrame(void) = 0;
    virtual void EngineStats_EndFrame(void) = 0;
    virtual void FireEvents() = 0;
    virtual int GetLeavesArea(unsigned short* pLeaves, int nLeaves) = 0;
    virtual bool DoesBoxTouchAreaFrustum(const vec3& mins, const vec3& maxs, int iArea) = 0;
    virtual int GetFrustumList(void** pList, int listMax) = 0;
    virtual void SetAudioState(const int& state) = 0;
    virtual int SentenceGroupPick(int groupIndex, char* name, int nameBufLen) = 0;
    virtual int SentenceGroupPickSequential(int groupIndex, char* name, int nameBufLen, int sentenceIndex, int reset) = 0;
    virtual int	SentenceIndexFromName(const char* pSentenceName) = 0;
    virtual const char* SentenceNameFromIndex(int sentenceIndex) = 0;
    virtual int SentenceGroupIndexFromName(const char* pGroupName) = 0;
    virtual const char* SentenceGroupNameFromIndex(int groupIndex) = 0;
    virtual float SentenceLength(int sentenceIndex) = 0;
    virtual void ComputeLighting(const vec3& pt, const vec3* pNormal, bool bClamp, vec3& color, vec3* pBoxColors = NULL) = 0;
    virtual void ActivateOccluder(int nOccluderIndex, bool bActive) = 0;
    virtual bool IsOccluded(const vec3& vecAbsMins, const vec3& vecAbsMaxs) = 0;
    virtual void* SaveAllocMemory(size_t num, size_t size) = 0;
    virtual void SaveFreeMemory(void* pSaveMem) = 0;
    virtual void* GetNetChannelInfo(void) = 0;
    virtual void DebugDrawPhysCollide(const void* pCollide, CMaterial* pMaterial, const matrix3x4& transform, float* color) = 0;
    virtual void CheckPoint(const char* pName) = 0;
    virtual void DrawPortals() = 0;
    virtual bool IsPlayingDemo(void) = 0;
    virtual bool IsRecordingDemo(void) = 0;
    virtual bool IsPlayingTimeDemo(void) = 0;
    virtual int GetDemoRecordingTick(void) = 0;
    virtual int GetDemoPlaybackTick(void) = 0;
    virtual int GetDemoPlaybackStartTick(void) = 0;
    virtual float GetDemoPlaybackTimeScale(void) = 0;
    virtual int GetDemoPlaybackTotalTicks(void) = 0;
    virtual bool IsPaused(void) = 0;
    virtual float GetTimescale(void) const = 0;
    virtual bool IsTakingScreenshot(void) = 0;
    virtual bool IsHLTV(void) = 0;
    virtual bool IsReplay(void) = 0;
    virtual bool IsLevelMainMenuBackground(void) = 0;
    virtual void GetMainMenuBackgroundName(char* dest, int destlen) = 0;
    virtual void SetOcclusionParameters(const int& params) = 0;
    virtual void GetUILanguage(char* dest, int destlen) = 0;
    virtual int IsSkyboxVisibleFromPoint(const vec3& vecPoint) = 0;
    virtual const char* GetMapEntitiesString() = 0;
    virtual bool IsInEditMode(void) = 0;
    virtual float GetScreenAspectRatio(int viewportWidth, int viewportHeight) = 0;
    virtual bool REMOVED_SteamRefreshLogin(const char* password, bool isSecure) = 0;
    virtual bool REMOVED_SteamProcessCall(bool& finished) = 0;
    virtual unsigned int	GetEngineBuildNumber() = 0; // engines build
    virtual const char* GetProductVersionString() = 0; // mods version number (steam.inf)
    virtual void GrabPreColorCorrectedFrame(int x, int y, int width, int height) = 0;
    virtual bool IsHammerRunning() const = 0;
    virtual void ExecuteClientCmd(const char* szCmdString) = 0;
    virtual bool MapHasHDRLighting(void) = 0;
    virtual int	GetAppID() = 0;
    virtual vec3 GetLightForPointFast(const vec3& pos, bool bClamp) = 0;
    virtual void ClientCmd_Unrestricted(const char* szCmdString) = 0;
    virtual void SetRestrictServerCommands(bool bRestrict) = 0;
    virtual void SetRestrictClientCommands(bool bRestrict) = 0;
    virtual void SetOverlayBindProxy(int iOverlayID, void* pBindProxy) = 0;
    virtual bool CopyFrameBufferToMaterial(const char* pMaterialName) = 0;
    virtual void ReadConfiguration(const int iController, const bool readDefault) = 0;
    virtual void SetAchievementMgr(void* pAchievementMgr) = 0;
    virtual void* GetAchievementMgr() = 0;
    virtual bool MapLoadFailed(void) = 0;
    virtual void SetMapLoadFailed(bool bState) = 0;
    virtual bool IsLowViolence() = 0;
    virtual const char* GetMostRecentSaveGame(void) = 0;
    virtual void SetMostRecentSaveGame(const char* lpszFilename) = 0;
    virtual void StartXboxExitingProcess() = 0;
    virtual bool IsSaveInProgress() = 0;
    virtual void OnStorageDeviceAttached(int iController) = 0;
    virtual void OnStorageDeviceDetached(int iController) = 0;
    virtual void WriteScreenshot(const char* pFilename) = 0;
    virtual void ResetDemoInterpolation(void) = 0;
    virtual int	GetActiveSplitScreenPlayerSlot() = 0;
    virtual int	SetActiveSplitScreenPlayerSlot(int slot) = 0;
    virtual bool SetLocalPlayerIsResolvable(char const* pchContext, int nLine, bool bResolvable) = 0;
    virtual bool IsLocalPlayerResolvable() = 0;
    virtual int	GetSplitScreenPlayer(int nSlot) = 0;
    virtual bool IsSplitScreenActive() = 0;
    virtual bool IsValidSplitScreenSlot(int nSlot) = 0;
    virtual int	FirstValidSplitScreenSlot() = 0; // -1 == invalid
    virtual int	NextValidSplitScreenSlot(int nPreviousSlot) = 0; // -1 == invalid
    virtual void* GetSinglePlayerSharedMemorySpace(const char* szName, int ent_num) = 0;
    virtual void ComputeLightingCube(const vec3& pt, bool bClamp, vec3* pBoxColors) = 0;
    virtual void RegisterDemoCustomDataCallback(void* szCallbackSaveID, void* pCallback) = 0;
    virtual void RecordDemoCustomData(void* pCallback, const void* pData, size_t iDataLength) = 0;
    virtual void SetPitchScale(float flPitchScale) = 0;
    virtual float GetPitchScale(void) = 0;
    virtual bool LoadFilmmaker() = 0;
    virtual void UnloadFilmmaker() = 0;
    virtual void SetLeafFlag(int nLeafIndex, int nFlagBits) = 0;
    virtual void RecalculateBSPLeafFlags(void) = 0;
    virtual bool DSPGetCurrentDASRoomNew(void) = 0;
    virtual bool DSPGetCurrentDASRoomChanged(void) = 0;
    virtual bool DSPGetCurrentDASRoomSkyAbove(void) = 0;
    virtual float DSPGetCurrentDASRoomSkyPercent(void) = 0;
    virtual void SetMixGroupOfCurrentMixer(const char* szgroupname, const char* szparam, float val, int setMixerType) = 0;
    virtual int GetMixLayerIndex(const char* szmixlayername) = 0;
    virtual void SetMixLayerLevel(int index, float level) = 0;
    virtual bool IsCreatingReslist() = 0;
    virtual bool IsCreatingXboxReslist() = 0;
    virtual void SetTimescale(float flTimescale) = 0;
    virtual void SetGamestatsData(void* pGamestatsData) = 0;
    virtual void* GetGamestatsData() = 0;
    virtual	const char* Key_LookupBindingEx(const char* pBinding, int iUserId = -1, int iStartCount = 0, int iAllowJoystick = -1) = 0;
    virtual void UpdateDAndELights(void) = 0;
    virtual int GetBugSubmissionCount() const = 0;
    virtual void ClearBugSubmissionCount() = 0;
    virtual bool DoesLevelContainWater() const = 0;
    virtual float GetServerSimulationFrameTime() const = 0;
    virtual void SolidMoved(class IClientEntity* pSolidEnt, class ICollideable* pSolidCollide, const vec3* pPrevAbsOrigin, bool accurateBboxTriggerChecks) = 0;
    virtual void TriggerMoved(class IClientEntity* pTriggerEnt, bool accurateBboxTriggerChecks) = 0;
    virtual void ComputeLeavesConnected(const vec3& vecOrigin, int nCount, const int* pLeafIndices, bool* pIsConnected) = 0;
    virtual bool IsInCommentaryMode(void) = 0;
    virtual void SetBlurFade(float amount) = 0;
    virtual bool IsTransitioningToLoad() = 0;
    virtual void SearchPathsChangedAfterInstall() = 0;
    virtual void ConfigureSystemLevel(int nCPULevel, int nGPULevel) = 0;
    virtual void SetConnectionPassword(char const* pchCurrentPW) = 0;
    virtual void* GetSteamAPIContext() = 0;
    virtual void SubmitStatRecord(char const* szMapName, void* uiBlobVersion, void* uiBlobSize, const void* pvBlob) = 0;
    virtual void ServerCmdKeyValues(CKeyValues* pKeyValues) = 0;
    virtual void PaintSurface(const model_t* model, const vec3& position, const Vector4D& color, float radius) = 0;
    virtual void EnablePaintmapRender() = 0;
    virtual void TracePaintSurface(const model_t* model, const vec3& position, float radius, void* surfColors) = 0;
    virtual void RemoveAllPaint() = 0;
    virtual bool IsActiveApp() = 0;
    virtual bool IsClientLocalToActiveServer() = 0;
    virtual void TickProgressBar() = 0;
    virtual int GetInputContext(int id) = 0;
    virtual void SetMouseWindowLock(bool bLockToWindow) = 0;
};

class CPlayerInfoManager {
public:
    /*0*/ virtual void* GetPlayerInfo(void*) = 0;
    /*1*/ virtual CGlobalVarsBase* GetGlobalVars(void) = 0;
};

class CHLClient {
public:
    /*0*/ virtual void* Init(void* (*)(char const*, int*), void* (*)(char const*, int*), CGlobalVarsBase*) = 0;
    /*1*/ virtual void* PostInit(void) = 0;
    /*2*/ virtual void* Shutdown(void) = 0;
    /*3*/ virtual void* ReplayInit(void* (*)(char const*, int*)) = 0;
    /*4*/ virtual void* ReplayPostInit(void) = 0;
    /*5*/ virtual void* LevelInitPreEntity(char const*) = 0;
    /*6*/ virtual void* LevelInitPostEntity(void) = 0;
    /*7*/ virtual void* LevelShutdown(void) = 0;
    /*8*/ virtual ClientClass* GetAllClasses(void) = 0;
    /*9*/ virtual void* HudVidInit(void) = 0;
    /*10*/ virtual void* HudProcessInput(bool) = 0;
    /*11*/ virtual void* HudUpdate(bool) = 0;
    /*12*/ virtual void* HudReset(void) = 0;
    /*13*/ virtual void* HudText(char const*) = 0;
    /*14*/ virtual void* IN_ActivateMouse(void) = 0;
    /*15*/ virtual void* IN_DeactivateMouse(void) = 0;
    /*16*/ virtual void* IN_Accumulate(void) = 0;
    /*17*/ virtual void* IN_ClearStates(void) = 0;
    /*18*/ virtual void* IN_IsKeyDown(char const*, bool&) = 0;
    /*19*/ virtual void* IN_OnMouseWheeled(int) = 0;
    /*20*/ virtual void* IN_KeyEvent(int, ButtonCode_t, char const*) = 0;
    /*21*/ virtual void* CreateMove(int, float, bool) = 0;
    /*22*/ virtual void* ExtraMouseSample(float, bool) = 0;
    /*23*/ virtual void* WriteUsercmdDeltaToBuffer(void*, int, int, bool) = 0;
    /*24*/ virtual void* EncodeUserCmdToBuffer(void*, int) = 0;
    /*25*/ virtual void* DecodeUserCmdFromBuffer(void*, int) = 0;
    /*26*/ virtual void* View_Render(RECT*) = 0;
    /*27*/ virtual void* RenderView(CViewSetup const&, int, int) = 0;
    /*28*/ virtual void* View_Fade(ScreenFade_t*) = 0;
    /*29*/ virtual void* SetCrosshairAngle(QAngle const&) = 0;
    /*30*/ virtual void* InitSprite(CEngineSprite*, char const*) = 0;
    /*31*/ virtual void* ShutdownSprite(CEngineSprite*) = 0;
    /*32*/ virtual void* GetSpriteSize(void)const = 0;
    /*33*/ virtual void* VoiceStatus(int, int) = 0;
    /*34*/ virtual void* InstallStringTableCallback(char const*) = 0;
    /*35*/ virtual void* FrameStageNotify(ClientFrameStage_t) = 0;
    /*36*/ virtual void* DispatchUserMessage(int, void*) = 0;
    /*37*/ virtual void* SaveInit(int) = 0;
    /*38*/ virtual void* SaveWriteFields(CSaveRestoreData*, char const*, void*, datamap_t*, typedescription_t*, int) = 0;
    /*39*/ virtual void* SaveReadFields(CSaveRestoreData*, char const*, void*, datamap_t*, typedescription_t*, int) = 0;
    /*40*/ virtual void* PreSave(CSaveRestoreData*) = 0;
    /*41*/ virtual void* Save(CSaveRestoreData*) = 0;
    /*42*/ virtual void* WriteSaveHeaders(CSaveRestoreData*) = 0;
    /*43*/ virtual void* ReadRestoreHeaders(CSaveRestoreData*) = 0;
    /*44*/ virtual void* Restore(CSaveRestoreData*, bool) = 0;
    /*45*/ virtual void* DispatchOnRestore(void) = 0;
    /*46*/ virtual void* GetStandardRecvProxies(void) = 0;
    /*47*/ virtual void* WriteSaveGameScreenshot(char const*) = 0;
    /*48*/ virtual void* EmitSentenceCloseCaption(char const*) = 0;
    /*49*/ virtual void* EmitCloseCaption(char const*, float) = 0;
    /*50*/ virtual void* CanRecordDemo(char*, int)const = 0;
    /*51*/ virtual void* OnDemoRecordStart(char const*) = 0;
    /*52*/ virtual void* OnDemoRecordStop(void) = 0;
    /*53*/ virtual void* OnDemoPlaybackStart(char const*) = 0;
    /*54*/ virtual void* OnDemoPlaybackStop(void) = 0;
    /*55*/ virtual void* ShouldDrawDropdownConsole(void) = 0;
    /*56*/ virtual void* GetScreenWidth(void) = 0;
    /*57*/ virtual void* GetScreenHeight(void) = 0;
    /*58*/ virtual void* WriteSaveGameScreenshotOfSize(char const*, int, int, bool, bool) = 0;
    /*59*/ virtual void* GetPlayerView(CViewSetup&) = 0;
    /*60*/ virtual void* SetupGameProperties(void*, void*) = 0;
    /*61*/ virtual void* GetPresenceID(char const*) = 0;
    /*62*/ virtual void* GetPropertyIdString(unsigned int) = 0;
    /*63*/ virtual void* GetPropertyDisplayString(unsigned int, unsigned int, char*, int) = 0;
    /*64*/ virtual void* InvalidateMdlCache(void) = 0;
    /*65*/ virtual void* IN_SetSampleTime(float) = 0;
    /*66*/ virtual void* ReloadFilesInList(IFileList*) = 0;
    /*67*/ virtual void* StartStatsReporting(void*, bool) = 0;
    /*68*/ virtual void* HandleUiToggle(void) = 0;
    /*69*/ virtual void* ShouldAllowConsole(void) = 0;
    /*70*/ virtual void* GetRenamedRecvTableInfos(void) = 0;
    /*71*/ virtual void* GetClientUIMouthInfo(void) = 0;
    /*72*/ virtual void* FileReceived(char const*, unsigned int) = 0;
    /*73*/ virtual void* TranslateEffectForVisionFilter(char const*, char const*) = 0;
    /*74*/ virtual void* ClientAdjustStartSoundParams(StartSoundParams_t&) = 0;
    /*75*/ virtual void* DisconnectAttempt(void) = 0;
    /*76*/ virtual void* IsConnectedUserInfoChangeAllowed(IConVar*) = 0;
};

CHLClient* chlclient;

int GetOffset(RecvTable* pTable, const char* const szVarName) {
    for (int n = 0; n < pTable->m_nProps; n++) {
        RecvProp Prop = pTable->m_pProps[n];
        if (Prop.m_pVarName == szVarName) return Prop.GetOffset();

        RecvTable* pTable = Prop.GetDataTable();
        if (pTable) {
            const int nOff = GetOffset(pTable, szVarName);
            if (nOff) return (nOff + Prop.GetOffset());
        }
    }
    return 0;
}

int GetNetVar(const char* const szClass, const char* const szVarName) {
    ClientClass* pCC = chlclient->GetAllClasses();
    while (pCC) {
        if (szClass == pCC->m_pNetworkName) {
            return GetOffset(pCC->m_pRecvTable, szVarName);
        }
        pCC = pCC->m_pNext;
    }
    return 0;
}

#define NETVAR(_name, type, table, name) inline type &_name() \
{ \
	static const int nOffset = GetNetVar(table, name); \
	return *reinterpret_cast<type *>(reinterpret_cast<uintptr_t>(this) + nOffset); \
}
#define NETVAR_OFFSET(_name, type, table, name, off) inline type &_name() \
{ \
	static const int nOffset = GetNetVar(table, name) + off; \
	return *reinterpret_cast<type *>(reinterpret_cast<uintptr_t>(this) + nOffset); \
}
#define OFFSET(type, var, off) type &var() \
{ \
	return *reinterpret_cast<type *>(reinterpret_cast<uintptr_t>(this) + off); \
}

class CSmokeStack {
public:
    NETVAR(m_SpreadSpeed, float, "CSmokeStack", "m_SpreadSpeed");
    NETVAR(m_Speed, float, "CSmokeStack", "m_Speed");
    NETVAR(m_StartSize, float, "CSmokeStack", "m_StartSize");
    NETVAR(m_EndSize, float, "CSmokeStack", "m_EndSize");
    NETVAR(m_Rate, float, "CSmokeStack", "m_Rate");
    NETVAR(m_JetLength, float, "CSmokeStack", "m_JetLength");
    NETVAR(m_bEmit, int, "CSmokeStack", "m_bEmit");
    NETVAR(m_flBaseSpread, float, "CSmokeStack", "m_flBaseSpread");
    NETVAR(m_flTwist, float, "CSmokeStack", "m_flTwist");
    NETVAR(m_flRollSpeed, float, "CSmokeStack", "m_flRollSpeed");
    NETVAR(m_iMaterialModel, int, "CSmokeStack", "m_iMaterialModel");
    NETVAR(m_vWind, vec3, "CSmokeStack", "m_vWind");
};

class CFireTrail {
public:
    NETVAR(m_nAttachment, int, "CFireTrail", "m_nAttachment");
    NETVAR(m_flLifetime, float, "CFireTrail", "m_flLifetime");
};

class SmokeTrail {
public:
    NETVAR(m_SpawnRate, float, "SmokeTrail", "m_SpawnRate");
    NETVAR(m_StartColor, vec3, "SmokeTrail", "m_StartColor");
    NETVAR(m_EndColor, vec3, "SmokeTrail", "m_EndColor");
    NETVAR(m_ParticleLifetime, float, "SmokeTrail", "m_ParticleLifetime");
    NETVAR(m_StopEmitTime, float, "SmokeTrail", "m_StopEmitTime");
    NETVAR(m_MinSpeed, float, "SmokeTrail", "m_MinSpeed");
    NETVAR(m_MaxSpeed, float, "SmokeTrail", "m_MaxSpeed");
    NETVAR(m_MinDirectedSpeed, float, "SmokeTrail", "m_MinDirectedSpeed");
    NETVAR(m_MaxDirectedSpeed, float, "SmokeTrail", "m_MaxDirectedSpeed");
    NETVAR(m_StartSize, float, "SmokeTrail", "m_StartSize");
    NETVAR(m_EndSize, float, "SmokeTrail", "m_EndSize");
    NETVAR(m_SpawnRadius, float, "SmokeTrail", "m_SpawnRadius");
    NETVAR(m_bEmit, int, "SmokeTrail", "m_bEmit");
    NETVAR(m_nAttachment, int, "SmokeTrail", "m_nAttachment");
    NETVAR(m_Opacity, float, "SmokeTrail", "m_Opacity");
};

class ParticleSmokeGrenade {
public:
    NETVAR(m_flSpawnTime, float, "ParticleSmokeGrenade", "m_flSpawnTime");
    NETVAR(m_FadeStartTime, float, "ParticleSmokeGrenade", "m_FadeStartTime");
    NETVAR(m_FadeEndTime, float, "ParticleSmokeGrenade", "m_FadeEndTime");
    NETVAR(m_CurrentStage, int, "ParticleSmokeGrenade", "m_CurrentStage");
};

class CParticleFire {
public:
    NETVAR(m_vOrigin, vec3, "CParticleFire", "m_vOrigin");
    NETVAR(m_vDirection, vec3, "CParticleFire", "m_vDirection");
};

class CWeaponXM1014 {
public:
    NETVAR(m_reloadState, int, "CWeaponXM1014", "m_reloadState");
};

class CWeaponUSP {
public:
    NETVAR(m_bSilencerOn, int, "CWeaponUSP", "m_bSilencerOn");
    NETVAR(m_flDoneSwitchingSilencer, float, "CWeaponUSP", "m_flDoneSwitchingSilencer");
};

class CWeaponM4A1 {
public:
    NETVAR(m_bSilencerOn, int, "CWeaponM4A1", "m_bSilencerOn");
    NETVAR(m_flDoneSwitchingSilencer, float, "CWeaponM4A1", "m_flDoneSwitchingSilencer");
};

class CWeaponM3 {
public:
    NETVAR(m_reloadState, int, "CWeaponM3", "m_reloadState");
};

class CKnife {
public:
    NETVAR(m_flSmackTime, float, "CKnife", "m_flSmackTime");
    NETVAR(LocalActiveWeaponKnifeData, RecvTable*, "CKnife", "LocalActiveWeaponKnifeData");
};

class CWeaponGlock {
public:
    NETVAR(m_bBurstMode, int, "CWeaponGlock", "m_bBurstMode");
    NETVAR(m_iBurstShotsRemaining, int, "CWeaponGlock", "m_iBurstShotsRemaining");
};

class CWeaponFamas {
public:
    NETVAR(m_bBurstMode, int, "CWeaponFamas", "m_bBurstMode");
    NETVAR(m_iBurstShotsRemaining, int, "CWeaponFamas", "m_iBurstShotsRemaining");
};

class CWeaponCSBase {
public:
    NETVAR(m_weaponMode, int, "CWeaponCSBase", "m_weaponMode");
    NETVAR(m_fAccuracyPenalty, float, "CWeaponCSBase", "m_fAccuracyPenalty");
};

class CC4 {
public:
    NETVAR(m_bStartedArming, int, "CC4", "m_bStartedArming");
    NETVAR(m_bBombPlacedAnimation, int, "CC4", "m_bBombPlacedAnimation");
    NETVAR(m_fArmedTime, float, "CC4", "m_fArmedTime");
};

class CBaseCSGrenade {
public:
    NETVAR(m_bRedraw, int, "CBaseCSGrenade", "m_bRedraw");
    NETVAR(m_bPinPulled, int, "CBaseCSGrenade", "m_bPinPulled");
    NETVAR(m_fThrowTime, float, "CBaseCSGrenade", "m_fThrowTime");
};

class CFootstepControl {
public:
    NETVAR(m_source, char*, "CFootstepControl", "m_source");
    NETVAR(m_destination, char*, "CFootstepControl", "m_destination");
};

class CCSGameRulesProxy {
public:
    NETVAR(m_bFreezePeriod, int, "CCSGameRulesProxy", "m_bFreezePeriod");
    NETVAR(m_iRoundTime, int, "CCSGameRulesProxy", "m_iRoundTime");
    NETVAR(m_fRoundStartTime, float, "CCSGameRulesProxy", "m_fRoundStartTime");
    NETVAR(m_flGameStartTime, float, "CCSGameRulesProxy", "m_flGameStartTime");
    NETVAR(m_iHostagesRemaining, int, "CCSGameRulesProxy", "m_iHostagesRemaining");
    NETVAR(m_bMapHasBombTarget, int, "CCSGameRulesProxy", "m_bMapHasBombTarget");
    NETVAR(m_bMapHasRescueZone, int, "CCSGameRulesProxy", "m_bMapHasRescueZone");
    NETVAR(m_bLogoMap, int, "CCSGameRulesProxy", "m_bLogoMap");
    NETVAR(m_bBlackMarket, int, "CCSGameRulesProxy", "m_bBlackMarket");
    NETVAR(m_bWinterHolidayActive, int, "CCSGameRulesProxy", "m_bWinterHolidayActive");
    NETVAR(cs_gamerules_data, RecvTable*, "CCSGameRulesProxy", "cs_gamerules_data");
};

class CTEPlantBomb {
public:
    NETVAR(m_vecOrigin, vec3, "CTEPlantBomb", "m_vecOrigin");
    NETVAR(m_iPlayer, int, "CTEPlantBomb", "m_iPlayer");
    NETVAR(m_option, int, "CTEPlantBomb", "m_option");
};

class CTEFireBullets {
public:
    NETVAR(m_vecOrigin, vec3, "CTEFireBullets", "m_vecOrigin");
    NETVAR(m_vecAngles, vec2, "CTEFireBullets", "m_vecAngles[0]");
    NETVAR(m_iWeaponID, int, "CTEFireBullets", "m_iWeaponID");
    NETVAR(m_iMode, int, "CTEFireBullets", "m_iMode");
    NETVAR(m_iSeed, int, "CTEFireBullets", "m_iSeed");
    NETVAR(m_iPlayer, int, "CTEFireBullets", "m_iPlayer");
    NETVAR(m_fInaccuracy, float, "CTEFireBullets", "m_fInaccuracy");
    NETVAR(m_fSpread, float, "CTEFireBullets", "m_fSpread");
};

class CPlantedC4 {
public:
    NETVAR(m_bBombTicking, int, "CPlantedC4", "m_bBombTicking");
    NETVAR(m_flC4Blow, float, "CPlantedC4", "m_flC4Blow");
    NETVAR(m_flTimerLength, float, "CPlantedC4", "m_flTimerLength");
    NETVAR(m_flDefuseLength, float, "CPlantedC4", "m_flDefuseLength");
    NETVAR(m_flDefuseCountDown, float, "CPlantedC4", "m_flDefuseCountDown");
};

class CCSPlayerResource {
public:
    NETVAR(m_iPlayerC4, int, "CCSPlayerResource", "m_iPlayerC4");
    NETVAR(m_iPlayerVIP, int, "CCSPlayerResource", "m_iPlayerVIP");
    NETVAR(m_vecC4, vec3, "CCSPlayerResource", "m_vecC4");
    NETVAR(m_bHostageAlive, RecvTable*, "CCSPlayerResource", "m_bHostageAlive");
    NETVAR(m_isHostageFollowingSomeone, RecvTable*, "CCSPlayerResource", "m_isHostageFollowingSomeone");
    NETVAR(m_iHostageEntityIDs, RecvTable*, "CCSPlayerResource", "m_iHostageEntityIDs");
    NETVAR(m_iHostageX, RecvTable*, "CCSPlayerResource", "m_iHostageX");
    NETVAR(m_iHostageY, RecvTable*, "CCSPlayerResource", "m_iHostageY");
    NETVAR(m_iHostageZ, RecvTable*, "CCSPlayerResource", "m_iHostageZ");
    NETVAR(m_bombsiteCenterA, vec3, "CCSPlayerResource", "m_bombsiteCenterA");
    NETVAR(m_bombsiteCenterB, vec3, "CCSPlayerResource", "m_bombsiteCenterB");
    NETVAR(m_hostageRescueX, RecvTable*, "CCSPlayerResource", "m_hostageRescueX");
    NETVAR(m_hostageRescueY, RecvTable*, "CCSPlayerResource", "m_hostageRescueY");
    NETVAR(m_hostageRescueZ, RecvTable*, "CCSPlayerResource", "m_hostageRescueZ");
    NETVAR(m_bBombSpotted, int, "CCSPlayerResource", "m_bBombSpotted");
    NETVAR(m_bPlayerSpotted, RecvTable*, "CCSPlayerResource", "m_bPlayerSpotted");
    NETVAR(m_iMVPs, RecvTable*, "CCSPlayerResource", "m_iMVPs");
    NETVAR(m_bHasDefuser, RecvTable*, "CCSPlayerResource", "m_bHasDefuser");
    NETVAR(m_szClan, RecvTable*, "CCSPlayerResource", "m_szClan");
};

class CCSPlayer {
public:
    NETVAR(m_flStamina, float, "CCSPlayer", "m_flStamina");
    NETVAR(m_iDirection, int, "CCSPlayer", "m_iDirection");
    NETVAR(m_iShotsFired, int, "CCSPlayer", "m_iShotsFired");
    NETVAR(m_flVelocityModifier, float, "CCSPlayer", "m_flVelocityModifier");
    //NETVAR(m_vecOrigin, vec3, "CCSPlayer", "m_vecOrigin");
    NETVAR(m_bPlayerDominated, RecvTable*, "CCSPlayer", "m_bPlayerDominated");
    NETVAR(m_bPlayerDominatingMe, RecvTable*, "CCSPlayer", "m_bPlayerDominatingMe");
    NETVAR(cslocaldata, RecvTable*, "CCSPlayer", "cslocaldata");
    NETVAR(csnonlocaldata, RecvTable*, "CCSPlayer", "csnonlocaldata");
    NETVAR(m_iAddonBits, int, "CCSPlayer", "m_iAddonBits");
    NETVAR(m_iPrimaryAddon, int, "CCSPlayer", "m_iPrimaryAddon");
    NETVAR(m_iSecondaryAddon, int, "CCSPlayer", "m_iSecondaryAddon");
    NETVAR(m_iThrowGrenadeCounter, int, "CCSPlayer", "m_iThrowGrenadeCounter");
    NETVAR(m_iPlayerState, int, "CCSPlayer", "m_iPlayerState");
    NETVAR(m_iAccount, int, "CCSPlayer", "m_iAccount");
    NETVAR(m_bInBombZone, int, "CCSPlayer", "m_bInBombZone");
    NETVAR(m_bInBuyZone, int, "CCSPlayer", "m_bInBuyZone");
    NETVAR(m_iClass, int, "CCSPlayer", "m_iClass");
    NETVAR(m_ArmorValue, int, "CCSPlayer", "m_ArmorValue");
    NETVAR(m_angEyeAngles, vec2, "CCSPlayer", "m_angEyeAngles[0]");
    NETVAR(m_bHasDefuser, int, "CCSPlayer", "m_bHasDefuser");
    NETVAR(m_bNightVisionOn, int, "CCSPlayer", "m_bNightVisionOn");
    NETVAR(m_bHasNightVision, int, "CCSPlayer", "m_bHasNightVision");
    NETVAR(m_bInHostageRescueZone, int, "CCSPlayer", "m_bInHostageRescueZone");
    NETVAR(m_bIsDefusing, int, "CCSPlayer", "m_bIsDefusing");
    NETVAR(m_bResumeZoom, int, "CCSPlayer", "m_bResumeZoom");
    NETVAR(m_iLastZoom, int, "CCSPlayer", "m_iLastZoom");
    NETVAR(m_bHasHelmet, int, "CCSPlayer", "m_bHasHelmet");
    NETVAR(m_vecRagdollVelocity, vec3, "CCSPlayer", "m_vecRagdollVelocity");
    NETVAR(m_flFlashDuration, float, "CCSPlayer", "m_flFlashDuration");
    NETVAR(m_flFlashMaxAlpha, float, "CCSPlayer", "m_flFlashMaxAlpha");
    NETVAR(m_iProgressBarDuration, int, "CCSPlayer", "m_iProgressBarDuration");
    NETVAR(m_flProgressBarStartTime, float, "CCSPlayer", "m_flProgressBarStartTime");
    NETVAR(m_hRagdoll, int, "CCSPlayer", "m_hRagdoll");
    NETVAR(m_cycleLatch, int, "CCSPlayer", "m_cycleLatch");
};

class CCSRagdoll {
public:
    NETVAR(m_vecOrigin, vec3, "CCSRagdoll", "m_vecOrigin");
    NETVAR(m_vecRagdollOrigin, vec3, "CCSRagdoll", "m_vecRagdollOrigin");
    NETVAR(m_hPlayer, int, "CCSRagdoll", "m_hPlayer");
    NETVAR(m_nModelIndex, int, "CCSRagdoll", "m_nModelIndex");
    NETVAR(m_nForceBone, int, "CCSRagdoll", "m_nForceBone");
    NETVAR(m_vecForce, vec3, "CCSRagdoll", "m_vecForce");
    NETVAR(m_vecRagdollVelocity, vec3, "CCSRagdoll", "m_vecRagdollVelocity");
    NETVAR(m_iDeathPose, int, "CCSRagdoll", "m_iDeathPose");
    NETVAR(m_iDeathFrame, int, "CCSRagdoll", "m_iDeathFrame");
    NETVAR(m_iTeamNum, int, "CCSRagdoll", "m_iTeamNum");
    NETVAR(m_bClientSideAnimation, int, "CCSRagdoll", "m_bClientSideAnimation");
};

class CTEPlayerAnimEvent {
public:
    NETVAR(m_hPlayer, int, "CTEPlayerAnimEvent", "m_hPlayer");
    NETVAR(m_iEvent, int, "CTEPlayerAnimEvent", "m_iEvent");
    NETVAR(m_nData, int, "CTEPlayerAnimEvent", "m_nData");
};

class CHostage {
public:
    NETVAR(m_isRescued, int, "CHostage", "m_isRescued");
    NETVAR(m_iHealth, int, "CHostage", "m_iHealth");
    NETVAR(m_iMaxHealth, int, "CHostage", "m_iMaxHealth");
    NETVAR(m_lifeState, int, "CHostage", "m_lifeState");
    NETVAR(m_leader, int, "CHostage", "m_leader");
};

class CBaseCSGrenadeProjectile {
public:
    NETVAR(m_vInitialVelocity, vec3, "CBaseCSGrenadeProjectile", "m_vInitialVelocity");
};

class CPoseController {
public:
    NETVAR(m_hProps, RecvTable*, "CPoseController", "m_hProps");
    NETVAR(m_chPoseIndex, RecvTable*, "CPoseController", "m_chPoseIndex");
    NETVAR(m_bPoseValueParity, int, "CPoseController", "m_bPoseValueParity");
    NETVAR(m_fPoseValue, float, "CPoseController", "m_fPoseValue");
    NETVAR(m_fInterpolationTime, float, "CPoseController", "m_fInterpolationTime");
    NETVAR(m_bInterpolationWrap, int, "CPoseController", "m_bInterpolationWrap");
    NETVAR(m_fCycleFrequency, float, "CPoseController", "m_fCycleFrequency");
    NETVAR(m_nFModType, int, "CPoseController", "m_nFModType");
    NETVAR(m_fFModTimeOffset, float, "CPoseController", "m_fFModTimeOffset");
    NETVAR(m_fFModRate, float, "CPoseController", "m_fFModRate");
    NETVAR(m_fFModAmplitude, float, "CPoseController", "m_fFModAmplitude");
};

class CFuncLadder {
public:
    NETVAR(m_vecPlayerMountPositionTop, vec3, "CFuncLadder", "m_vecPlayerMountPositionTop");
    NETVAR(m_vecPlayerMountPositionBottom, vec3, "CFuncLadder", "m_vecPlayerMountPositionBottom");
    NETVAR(m_vecLadderDir, vec3, "CFuncLadder", "m_vecLadderDir");
    NETVAR(m_bFakeLadder, int, "CFuncLadder", "m_bFakeLadder");
};

class CEnvDetailController {
public:
    NETVAR(m_flFadeStartDist, float, "CEnvDetailController", "m_flFadeStartDist");
    NETVAR(m_flFadeEndDist, float, "CEnvDetailController", "m_flFadeEndDist");
};

class CWorld {
public:
    NETVAR(m_flWaveHeight, float, "CWorld", "m_flWaveHeight");
    NETVAR(m_WorldMins, vec3, "CWorld", "m_WorldMins");
    NETVAR(m_WorldMaxs, vec3, "CWorld", "m_WorldMaxs");
    NETVAR(m_bStartDark, int, "CWorld", "m_bStartDark");
    NETVAR(m_flMaxOccludeeArea, float, "CWorld", "m_flMaxOccludeeArea");
    NETVAR(m_flMinOccluderArea, float, "CWorld", "m_flMinOccluderArea");
    NETVAR(m_flMaxPropScreenSpaceWidth, float, "CWorld", "m_flMaxPropScreenSpaceWidth");
    NETVAR(m_flMinPropScreenSpaceWidth, float, "CWorld", "m_flMinPropScreenSpaceWidth");
    NETVAR(m_iszDetailSpriteMaterial, char*, "CWorld", "m_iszDetailSpriteMaterial");
    NETVAR(m_bColdWorld, int, "CWorld", "m_bColdWorld");
};

class CVoteController {
public:
    NETVAR(m_iActiveIssueIndex, int, "CVoteController", "m_iActiveIssueIndex");
    NETVAR(m_nVoteIdx, int, "CVoteController", "m_nVoteIdx");
    NETVAR(m_iOnlyTeamToVote, int, "CVoteController", "m_iOnlyTeamToVote");
    NETVAR(m_nVoteOptionCount, RecvTable*, "CVoteController", "m_nVoteOptionCount");
    NETVAR(m_nPotentialVotes, int, "CVoteController", "m_nPotentialVotes");
    NETVAR(m_bIsYesNoVote, int, "CVoteController", "m_bIsYesNoVote");
};

class CVGuiScreen {
public:
    NETVAR(m_flWidth, float, "CVGuiScreen", "m_flWidth");
    NETVAR(m_flHeight, float, "CVGuiScreen", "m_flHeight");
    NETVAR(m_fScreenFlags, int, "CVGuiScreen", "m_fScreenFlags");
    NETVAR(m_nPanelName, int, "CVGuiScreen", "m_nPanelName");
    NETVAR(m_nAttachmentIndex, int, "CVGuiScreen", "m_nAttachmentIndex");
    NETVAR(m_nOverlayMaterial, int, "CVGuiScreen", "m_nOverlayMaterial");
    NETVAR(m_hPlayerOwner, int, "CVGuiScreen", "m_hPlayerOwner");
};

class CTeam {
public:
    NETVAR(m_iTeamNum, int, "CTeam", "m_iTeamNum");
    NETVAR(m_iScore, int, "CTeam", "m_iScore");
    NETVAR(m_iRoundsWon, int, "CTeam", "m_iRoundsWon");
    NETVAR(m_szTeamname, char*, "CTeam", "m_szTeamname");
    NETVAR(player_array_element, int, "CTeam", "player_array_element");
};

class CSun {
public:
    NETVAR(m_clrRender, int, "CSun", "m_clrRender");
    NETVAR(m_clrOverlay, int, "CSun", "m_clrOverlay");
    NETVAR(m_vDirection, vec3, "CSun", "m_vDirection");
    NETVAR(m_bOn, int, "CSun", "m_bOn");
    NETVAR(m_nSize, int, "CSun", "m_nSize");
    NETVAR(m_nOverlaySize, int, "CSun", "m_nOverlaySize");
    NETVAR(m_nMaterial, int, "CSun", "m_nMaterial");
    NETVAR(m_nOverlayMaterial, int, "CSun", "m_nOverlayMaterial");
    NETVAR(HDRColorScale, float, "CSun", "HDRColorScale");
};

class CParticlePerformanceMonitor {
public:
    NETVAR(m_bMeasurePerf, int, "CParticlePerformanceMonitor", "m_bMeasurePerf");
    NETVAR(m_bDisplayPerf, int, "CParticlePerformanceMonitor", "m_bDisplayPerf");
};

class CSpotlightEnd {
public:
    NETVAR(m_flLightScale, float, "CSpotlightEnd", "m_flLightScale");
    NETVAR(m_Radius, float, "CSpotlightEnd", "m_Radius");
};

class CSlideshowDisplay {
public:
    NETVAR(m_bEnabled, int, "CSlideshowDisplay", "m_bEnabled");
    NETVAR(m_szDisplayText, char*, "CSlideshowDisplay", "m_szDisplayText");
    NETVAR(m_szSlideshowDirectory, char*, "CSlideshowDisplay", "m_szSlideshowDirectory");
    NETVAR(m_chCurrentSlideLists, RecvTable*, "CSlideshowDisplay", "m_chCurrentSlideLists");
    NETVAR(m_fMinSlideTime, float, "CSlideshowDisplay", "m_fMinSlideTime");
    NETVAR(m_fMaxSlideTime, float, "CSlideshowDisplay", "m_fMaxSlideTime");
    NETVAR(m_iCycleType, int, "CSlideshowDisplay", "m_iCycleType");
    NETVAR(m_bNoListRepeats, int, "CSlideshowDisplay", "m_bNoListRepeats");
};

class CShadowControl {
public:
    NETVAR(m_shadowDirection, vec3, "CShadowControl", "m_shadowDirection");
    NETVAR(m_shadowColor, int, "CShadowControl", "m_shadowColor");
    NETVAR(m_flShadowMaxDist, float, "CShadowControl", "m_flShadowMaxDist");
    NETVAR(m_bDisableShadows, int, "CShadowControl", "m_bDisableShadows");
};

class CSceneEntity {
public:
    NETVAR(m_nSceneStringIndex, int, "CSceneEntity", "m_nSceneStringIndex");
    NETVAR(m_bIsPlayingBack, int, "CSceneEntity", "m_bIsPlayingBack");
    NETVAR(m_bPaused, int, "CSceneEntity", "m_bPaused");
    NETVAR(m_bMultiplayer, int, "CSceneEntity", "m_bMultiplayer");
    NETVAR(m_flForceClientTime, float, "CSceneEntity", "m_flForceClientTime");
    NETVAR(m_hActorList, RecvTable*, "CSceneEntity", "m_hActorList");
};

class CPointWorldText {
public:
    NETVAR(m_szText, char*, "CPointWorldText", "m_szText");
    NETVAR(m_flTextSize, float, "CPointWorldText", "m_flTextSize");
    NETVAR(m_flTextSpacingX, float, "CPointWorldText", "m_flTextSpacingX");
    NETVAR(m_flTextSpacingY, float, "CPointWorldText", "m_flTextSpacingY");
    NETVAR(m_colTextColor, int, "CPointWorldText", "m_colTextColor");
    NETVAR(m_nOrientation, int, "CPointWorldText", "m_nOrientation");
    NETVAR(m_nFont, int, "CPointWorldText", "m_nFont");
    NETVAR(m_bRainbow, int, "CPointWorldText", "m_bRainbow");
};

class CPointCommentaryNode {
public:
    NETVAR(m_bActive, int, "CPointCommentaryNode", "m_bActive");
    NETVAR(m_flStartTime, float, "CPointCommentaryNode", "m_flStartTime");
    NETVAR(m_iszCommentaryFile, char*, "CPointCommentaryNode", "m_iszCommentaryFile");
    NETVAR(m_iszCommentaryFileNoHDR, char*, "CPointCommentaryNode", "m_iszCommentaryFileNoHDR");
    NETVAR(m_iszSpeakers, char*, "CPointCommentaryNode", "m_iszSpeakers");
    NETVAR(m_iNodeNumber, int, "CPointCommentaryNode", "m_iNodeNumber");
    NETVAR(m_iNodeNumberMax, int, "CPointCommentaryNode", "m_iNodeNumberMax");
    NETVAR(m_hViewPosition, int, "CPointCommentaryNode", "m_hViewPosition");
};

class CPointCamera {
public:
    NETVAR(m_FOV, float, "CPointCamera", "m_FOV");
    NETVAR(m_Resolution, float, "CPointCamera", "m_Resolution");
    NETVAR(m_bFogEnable, int, "CPointCamera", "m_bFogEnable");
    NETVAR(m_FogColor, int, "CPointCamera", "m_FogColor");
    NETVAR(m_flFogStart, float, "CPointCamera", "m_flFogStart");
    NETVAR(m_flFogEnd, float, "CPointCamera", "m_flFogEnd");
    NETVAR(m_flFogMaxDensity, float, "CPointCamera", "m_flFogMaxDensity");
    NETVAR(m_bFogRadial, int, "CPointCamera", "m_bFogRadial");
    NETVAR(m_bActive, int, "CPointCamera", "m_bActive");
    NETVAR(m_bUseScreenAspectRatio, int, "CPointCamera", "m_bUseScreenAspectRatio");
};

class CPlayerResource {
public:
    NETVAR(m_iPing, RecvTable*, "CPlayerResource", "m_iPing");
    NETVAR(m_iScore, RecvTable*, "CPlayerResource", "m_iScore");
    NETVAR(m_iDeaths, RecvTable*, "CPlayerResource", "m_iDeaths");
    NETVAR(m_bConnected, RecvTable*, "CPlayerResource", "m_bConnected");
    NETVAR(m_iTeam, RecvTable*, "CPlayerResource", "m_iTeam");
    NETVAR(m_bAlive, RecvTable*, "CPlayerResource", "m_bAlive");
    NETVAR(m_iHealth, RecvTable*, "CPlayerResource", "m_iHealth");
    NETVAR(m_iAccountID, RecvTable*, "CPlayerResource", "m_iAccountID");
    NETVAR(m_bValid, RecvTable*, "CPlayerResource", "m_bValid");
    NETVAR(m_iUserID, RecvTable*, "CPlayerResource", "m_iUserID");
};

class CPlasma {
public:
    NETVAR(m_flStartScale, float, "CPlasma", "m_flStartScale");
    NETVAR(m_flScale, float, "CPlasma", "m_flScale");
    NETVAR(m_flScaleTime, float, "CPlasma", "m_flScaleTime");
    NETVAR(m_nFlags, int, "CPlasma", "m_nFlags");
    NETVAR(m_nPlasmaModelIndex, int, "CPlasma", "m_nPlasmaModelIndex");
    NETVAR(m_nPlasmaModelIndex2, int, "CPlasma", "m_nPlasmaModelIndex2");
    NETVAR(m_nGlowModelIndex, int, "CPlasma", "m_nGlowModelIndex");
};

class CPhysicsProp {
public:
    NETVAR(m_bAwake, int, "CPhysicsProp", "m_bAwake");
};

class CPhysBox {
public:
    NETVAR(m_mass, float, "CPhysBox", "m_mass");
};

class CParticleSystem {
public:
    NETVAR(m_vecOrigin, vec3, "CParticleSystem", "m_vecOrigin");
    NETVAR(m_hOwnerEntity, int, "CParticleSystem", "m_hOwnerEntity");
    NETVAR(moveparent, int, "CParticleSystem", "moveparent");
    NETVAR(m_iParentAttachment, int, "CParticleSystem", "m_iParentAttachment");
    NETVAR(m_angRotation, vec3, "CParticleSystem", "m_angRotation");
    NETVAR(m_iEffectIndex, int, "CParticleSystem", "m_iEffectIndex");
    NETVAR(m_bActive, int, "CParticleSystem", "m_bActive");
    NETVAR(m_flStartTime, float, "CParticleSystem", "m_flStartTime");
    NETVAR(m_hControlPointEnts, RecvTable*, "CParticleSystem", "m_hControlPointEnts");
    NETVAR(m_iControlPointParents, RecvTable*, "CParticleSystem", "m_iControlPointParents");
    NETVAR(m_bWeatherEffect, int, "CParticleSystem", "m_bWeatherEffect");
};

class CMaterialModifyControl {
public:
    NETVAR(m_szMaterialName, char*, "CMaterialModifyControl", "m_szMaterialName");
    NETVAR(m_szMaterialVar, char*, "CMaterialModifyControl", "m_szMaterialVar");
    NETVAR(m_szMaterialVarValue, char*, "CMaterialModifyControl", "m_szMaterialVarValue");
    NETVAR(m_iFrameStart, int, "CMaterialModifyControl", "m_iFrameStart");
    NETVAR(m_iFrameEnd, int, "CMaterialModifyControl", "m_iFrameEnd");
    NETVAR(m_bWrap, int, "CMaterialModifyControl", "m_bWrap");
    NETVAR(m_flFramerate, float, "CMaterialModifyControl", "m_flFramerate");
    NETVAR(m_bNewAnimCommandsSemaphore, int, "CMaterialModifyControl", "m_bNewAnimCommandsSemaphore");
    NETVAR(m_flFloatLerpStartValue, float, "CMaterialModifyControl", "m_flFloatLerpStartValue");
    NETVAR(m_flFloatLerpEndValue, float, "CMaterialModifyControl", "m_flFloatLerpEndValue");
    NETVAR(m_flFloatLerpTransitionTime, float, "CMaterialModifyControl", "m_flFloatLerpTransitionTime");
    NETVAR(m_bFloatLerpWrap, int, "CMaterialModifyControl", "m_bFloatLerpWrap");
    NETVAR(m_nModifyMode, int, "CMaterialModifyControl", "m_nModifyMode");
};

class CLightGlow {
public:
    NETVAR(m_clrRender, int, "CLightGlow", "m_clrRender");
    NETVAR(m_nHorizontalSize, int, "CLightGlow", "m_nHorizontalSize");
    NETVAR(m_nVerticalSize, int, "CLightGlow", "m_nVerticalSize");
    NETVAR(m_nMinDist, int, "CLightGlow", "m_nMinDist");
    NETVAR(m_nMaxDist, int, "CLightGlow", "m_nMaxDist");
    NETVAR(m_nOuterMaxDist, int, "CLightGlow", "m_nOuterMaxDist");
    NETVAR(m_spawnflags, int, "CLightGlow", "m_spawnflags");
    NETVAR(m_vecOrigin, vec3, "CLightGlow", "m_vecOrigin");
    NETVAR(m_angRotation, vec3, "CLightGlow", "m_angRotation");
    NETVAR(moveparent, int, "CLightGlow", "moveparent");
    NETVAR(m_flGlowProxySize, float, "CLightGlow", "m_flGlowProxySize");
    NETVAR(HDRColorScale, float, "CLightGlow", "HDRColorScale");
};

class CInfoOverlayAccessor {
public:
    NETVAR(m_iTextureFrameIndex, int, "CInfoOverlayAccessor", "m_iTextureFrameIndex");
    NETVAR(m_iOverlayID, int, "CInfoOverlayAccessor", "m_iOverlayID");
};

class CFuncSmokeVolume {
public:
    NETVAR(m_Color1, int, "CFuncSmokeVolume", "m_Color1");
    NETVAR(m_Color2, int, "CFuncSmokeVolume", "m_Color2");
    NETVAR(m_MaterialName, char*, "CFuncSmokeVolume", "m_MaterialName");
    NETVAR(m_ParticleDrawWidth, float, "CFuncSmokeVolume", "m_ParticleDrawWidth");
    NETVAR(m_ParticleSpacingDistance, float, "CFuncSmokeVolume", "m_ParticleSpacingDistance");
    NETVAR(m_DensityRampSpeed, float, "CFuncSmokeVolume", "m_DensityRampSpeed");
    NETVAR(m_RotationSpeed, float, "CFuncSmokeVolume", "m_RotationSpeed");
    NETVAR(m_MovementSpeed, float, "CFuncSmokeVolume", "m_MovementSpeed");
    NETVAR(m_Density, float, "CFuncSmokeVolume", "m_Density");
    NETVAR(m_spawnflags, int, "CFuncSmokeVolume", "m_spawnflags");
    NETVAR(m_vecMinsPreScaled, vec3, "CFuncSmokeVolume", "m_vecMinsPreScaled");
    NETVAR(m_vecMaxsPreScaled, vec3, "CFuncSmokeVolume", "m_vecMaxsPreScaled");
    NETVAR(m_vecMins, vec3, "CFuncSmokeVolume", "m_vecMins");
    NETVAR(m_vecMaxs, vec3, "CFuncSmokeVolume", "m_vecMaxs");
    NETVAR(m_nSolidType, int, "CFuncSmokeVolume", "m_nSolidType");
    NETVAR(m_usSolidFlags, int, "CFuncSmokeVolume", "m_usSolidFlags");
    NETVAR(m_nSurroundType, int, "CFuncSmokeVolume", "m_nSurroundType");
    NETVAR(m_triggerBloat, int, "CFuncSmokeVolume", "m_triggerBloat");
    NETVAR(m_bUniformTriggerBloat, int, "CFuncSmokeVolume", "m_bUniformTriggerBloat");
    NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, vec3, "CFuncSmokeVolume", "m_vecSpecifiedSurroundingMinsPreScaled");
    NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, vec3, "CFuncSmokeVolume", "m_vecSpecifiedSurroundingMaxsPreScaled");
    NETVAR(m_vecSpecifiedSurroundingMins, vec3, "CFuncSmokeVolume", "m_vecSpecifiedSurroundingMins");
    NETVAR(m_vecSpecifiedSurroundingMaxs, vec3, "CFuncSmokeVolume", "m_vecSpecifiedSurroundingMaxs");
    NETVAR(m_Collision, RecvTable*, "CFuncSmokeVolume", "m_Collision");
};

class CFuncRotating {
public:
    NETVAR(m_vecOrigin, vec3, "CFuncRotating", "m_vecOrigin");
    NETVAR(m_angRotation, vec3, "CFuncRotating", "m_angRotation[0]");
    NETVAR(m_flSimulationTime, int, "CFuncRotating", "m_flSimulationTime");
};

class CFuncOccluder {
public:
    NETVAR(m_bActive, int, "CFuncOccluder", "m_bActive");
    NETVAR(m_nOccluderIndex, int, "CFuncOccluder", "m_nOccluderIndex");
};

class CFunc_LOD {
public:
    NETVAR(m_fDisappearDist, float, "CFunc_LOD", "m_fDisappearDist");
};

class CTEDust {
public:
    NETVAR(m_flSize, float, "CTEDust", "m_flSize");
    NETVAR(m_flSpeed, float, "CTEDust", "m_flSpeed");
    NETVAR(m_vecDirection, vec3, "CTEDust", "m_vecDirection");
};

class CFunc_Dust {
public:
    NETVAR(m_Color, int, "CFunc_Dust", "m_Color");
    NETVAR(m_SpawnRate, int, "CFunc_Dust", "m_SpawnRate");
    NETVAR(m_flSizeMin, float, "CFunc_Dust", "m_flSizeMin");
    NETVAR(m_flSizeMax, float, "CFunc_Dust", "m_flSizeMax");
    NETVAR(m_LifetimeMin, int, "CFunc_Dust", "m_LifetimeMin");
    NETVAR(m_LifetimeMax, int, "CFunc_Dust", "m_LifetimeMax");
    NETVAR(m_DustFlags, int, "CFunc_Dust", "m_DustFlags");
    NETVAR(m_SpeedMax, int, "CFunc_Dust", "m_SpeedMax");
    NETVAR(m_DistMax, int, "CFunc_Dust", "m_DistMax");
    NETVAR(m_nModelIndex, int, "CFunc_Dust", "m_nModelIndex");
    NETVAR(m_FallSpeed, float, "CFunc_Dust", "m_FallSpeed");
    NETVAR(m_vecMinsPreScaled, vec3, "CFunc_Dust", "m_vecMinsPreScaled");
    NETVAR(m_vecMaxsPreScaled, vec3, "CFunc_Dust", "m_vecMaxsPreScaled");
    NETVAR(m_vecMins, vec3, "CFunc_Dust", "m_vecMins");
    NETVAR(m_vecMaxs, vec3, "CFunc_Dust", "m_vecMaxs");
    NETVAR(m_nSolidType, int, "CFunc_Dust", "m_nSolidType");
    NETVAR(m_usSolidFlags, int, "CFunc_Dust", "m_usSolidFlags");
    NETVAR(m_nSurroundType, int, "CFunc_Dust", "m_nSurroundType");
    NETVAR(m_triggerBloat, int, "CFunc_Dust", "m_triggerBloat");
    NETVAR(m_bUniformTriggerBloat, int, "CFunc_Dust", "m_bUniformTriggerBloat");
    NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, vec3, "CFunc_Dust", "m_vecSpecifiedSurroundingMinsPreScaled");
    NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, vec3, "CFunc_Dust", "m_vecSpecifiedSurroundingMaxsPreScaled");
    NETVAR(m_vecSpecifiedSurroundingMins, vec3, "CFunc_Dust", "m_vecSpecifiedSurroundingMins");
    NETVAR(m_vecSpecifiedSurroundingMaxs, vec3, "CFunc_Dust", "m_vecSpecifiedSurroundingMaxs");
    NETVAR(m_Collision, RecvTable*, "CFunc_Dust", "m_Collision");
};

class CFuncConveyor {
public:
    NETVAR(m_flConveyorSpeed, float, "CFuncConveyor", "m_flConveyorSpeed");
};

class CBreakableSurface {
public:
    NETVAR(m_nNumWide, int, "CBreakableSurface", "m_nNumWide");
    NETVAR(m_nNumHigh, int, "CBreakableSurface", "m_nNumHigh");
    NETVAR(m_flPanelWidth, float, "CBreakableSurface", "m_flPanelWidth");
    NETVAR(m_flPanelHeight, float, "CBreakableSurface", "m_flPanelHeight");
    NETVAR(m_vNormal, vec3, "CBreakableSurface", "m_vNormal");
    NETVAR(m_vCorner, vec3, "CBreakableSurface", "m_vCorner");
    NETVAR(m_bIsBroken, int, "CBreakableSurface", "m_bIsBroken");
    NETVAR(m_nSurfaceType, int, "CBreakableSurface", "m_nSurfaceType");
    NETVAR(m_RawPanelBitVec, RecvTable*, "CBreakableSurface", "m_RawPanelBitVec");
};

class CFuncAreaPortalWindow {
public:
    NETVAR(m_flFadeStartDist, float, "CFuncAreaPortalWindow", "m_flFadeStartDist");
    NETVAR(m_flFadeDist, float, "CFuncAreaPortalWindow", "m_flFadeDist");
    NETVAR(m_flTranslucencyLimit, float, "CFuncAreaPortalWindow", "m_flTranslucencyLimit");
    NETVAR(m_iBackgroundModelIndex, int, "CFuncAreaPortalWindow", "m_iBackgroundModelIndex");
};

class CFish {
public:
    NETVAR(m_poolOrigin, vec3, "CFish", "m_poolOrigin");
    NETVAR(m_x, float, "CFish", "m_x");
    NETVAR(m_y, float, "CFish", "m_y");
    NETVAR(m_z, float, "CFish", "m_z");
    NETVAR(m_angle, float, "CFish", "m_angle");
    NETVAR(m_nModelIndex, int, "CFish", "m_nModelIndex");
    NETVAR(m_lifeState, int, "CFish", "m_lifeState");
    NETVAR(m_waterLevel, float, "CFish", "m_waterLevel");
};

class CEntityFlame {
public:
    NETVAR(m_hEntAttached, int, "CEntityFlame", "m_hEntAttached");
};

class CFireSmoke {
public:
    NETVAR(m_flStartScale, float, "CFireSmoke", "m_flStartScale");
    NETVAR(m_flScale, float, "CFireSmoke", "m_flScale");
    NETVAR(m_flScaleTime, float, "CFireSmoke", "m_flScaleTime");
    NETVAR(m_nFlags, int, "CFireSmoke", "m_nFlags");
    NETVAR(m_nFlameModelIndex, int, "CFireSmoke", "m_nFlameModelIndex");
    NETVAR(m_nFlameFromAboveModelIndex, int, "CFireSmoke", "m_nFlameFromAboveModelIndex");
};

class CEnvTonemapController {
public:
    NETVAR(m_bUseCustomAutoExposureMin, int, "CEnvTonemapController", "m_bUseCustomAutoExposureMin");
    NETVAR(m_bUseCustomAutoExposureMax, int, "CEnvTonemapController", "m_bUseCustomAutoExposureMax");
    NETVAR(m_bUseCustomBloomScale, int, "CEnvTonemapController", "m_bUseCustomBloomScale");
    NETVAR(m_flCustomAutoExposureMin, float, "CEnvTonemapController", "m_flCustomAutoExposureMin");
    NETVAR(m_flCustomAutoExposureMax, float, "CEnvTonemapController", "m_flCustomAutoExposureMax");
    NETVAR(m_flCustomBloomScale, float, "CEnvTonemapController", "m_flCustomBloomScale");
    NETVAR(m_flCustomBloomScaleMinimum, float, "CEnvTonemapController", "m_flCustomBloomScaleMinimum");
};

class CEnvScreenEffect {
public:
    NETVAR(m_flDuration, float, "CEnvScreenEffect", "m_flDuration");
    NETVAR(m_nType, int, "CEnvScreenEffect", "m_nType");
};

class CEnvProjectedTexture {
public:
    NETVAR(m_hTargetEntity, int, "CEnvProjectedTexture", "m_hTargetEntity");
    NETVAR(m_bState, int, "CEnvProjectedTexture", "m_bState");
    NETVAR(m_flLightFOV, float, "CEnvProjectedTexture", "m_flLightFOV");
    NETVAR(m_bEnableShadows, int, "CEnvProjectedTexture", "m_bEnableShadows");
    NETVAR(m_bLightOnlyTarget, int, "CEnvProjectedTexture", "m_bLightOnlyTarget");
    NETVAR(m_bLightWorld, int, "CEnvProjectedTexture", "m_bLightWorld");
    NETVAR(m_bCameraSpace, int, "CEnvProjectedTexture", "m_bCameraSpace");
    NETVAR(m_LinearFloatLightColor, vec3, "CEnvProjectedTexture", "m_LinearFloatLightColor");
    NETVAR(m_flAmbient, float, "CEnvProjectedTexture", "m_flAmbient");
    NETVAR(m_SpotlightTextureName, char*, "CEnvProjectedTexture", "m_SpotlightTextureName");
    NETVAR(m_nSpotlightTextureFrame, int, "CEnvProjectedTexture", "m_nSpotlightTextureFrame");
    NETVAR(m_flNearZ, float, "CEnvProjectedTexture", "m_flNearZ");
    NETVAR(m_flFarZ, float, "CEnvProjectedTexture", "m_flFarZ");
    NETVAR(m_nShadowQuality, int, "CEnvProjectedTexture", "m_nShadowQuality");
};

class CEnvParticleScript {
public:
    NETVAR(m_flSequenceScale, float, "CEnvParticleScript", "m_flSequenceScale");
};

class CEntityParticleTrail {
public:
    NETVAR(m_iMaterialName, int, "CEntityParticleTrail", "m_iMaterialName");
    NETVAR(m_flLifetime, float, "CEntityParticleTrail", "m_flLifetime");
    NETVAR(m_flStartSize, float, "CEntityParticleTrail", "m_flStartSize");
    NETVAR(m_flEndSize, float, "CEntityParticleTrail", "m_flEndSize");
    NETVAR(m_Info, RecvTable*, "CEntityParticleTrail", "m_Info");
    NETVAR(m_hConstraintEntity, int, "CEntityParticleTrail", "m_hConstraintEntity");
};

class CEntityDissolve {
public:
    NETVAR(m_flStartTime, float, "CEntityDissolve", "m_flStartTime");
    NETVAR(m_flFadeOutStart, float, "CEntityDissolve", "m_flFadeOutStart");
    NETVAR(m_flFadeOutLength, float, "CEntityDissolve", "m_flFadeOutLength");
    NETVAR(m_flFadeOutModelStart, float, "CEntityDissolve", "m_flFadeOutModelStart");
    NETVAR(m_flFadeOutModelLength, float, "CEntityDissolve", "m_flFadeOutModelLength");
    NETVAR(m_flFadeInStart, float, "CEntityDissolve", "m_flFadeInStart");
    NETVAR(m_flFadeInLength, float, "CEntityDissolve", "m_flFadeInLength");
    NETVAR(m_nDissolveType, int, "CEntityDissolve", "m_nDissolveType");
    NETVAR(m_vDissolverOrigin, vec3, "CEntityDissolve", "m_vDissolverOrigin");
    NETVAR(m_nMagnitude, int, "CEntityDissolve", "m_nMagnitude");
};

class CDynamicLight {
public:
    NETVAR(m_Flags, int, "CDynamicLight", "m_Flags");
    NETVAR(m_LightStyle, int, "CDynamicLight", "m_LightStyle");
    NETVAR(m_Radius, float, "CDynamicLight", "m_Radius");
    NETVAR(m_Exponent, int, "CDynamicLight", "m_Exponent");
    NETVAR(m_InnerAngle, float, "CDynamicLight", "m_InnerAngle");
    NETVAR(m_OuterAngle, float, "CDynamicLight", "m_OuterAngle");
    NETVAR(m_SpotRadius, float, "CDynamicLight", "m_SpotRadius");
};

class CColorCorrectionVolume {
public:
    NETVAR(m_Weight, float, "CColorCorrectionVolume", "m_Weight");
    NETVAR(m_lookupFilename, char*, "CColorCorrectionVolume", "m_lookupFilename");
};

class CColorCorrection {
public:
    NETVAR(m_vecOrigin, vec3, "CColorCorrection", "m_vecOrigin");
    NETVAR(m_minFalloff, float, "CColorCorrection", "m_minFalloff");
    NETVAR(m_maxFalloff, float, "CColorCorrection", "m_maxFalloff");
    NETVAR(m_flCurWeight, float, "CColorCorrection", "m_flCurWeight");
    NETVAR(m_netLookupFilename, char*, "CColorCorrection", "m_netLookupFilename");
    NETVAR(m_bEnabled, int, "CColorCorrection", "m_bEnabled");
};

class CBasePlayer {
public:
    NETVAR(m_chAreaBits, RecvTable*, "CBasePlayer", "m_chAreaBits");
    NETVAR(m_chAreaPortalBits, RecvTable*, "CBasePlayer", "m_chAreaPortalBits");
    NETVAR(m_iHideHUD, int, "CBasePlayer", "m_iHideHUD");
    NETVAR(m_flFOVRate, float, "CBasePlayer", "m_flFOVRate");
    NETVAR(m_bDucked, int, "CBasePlayer", "m_bDucked");
    NETVAR(m_bDucking, int, "CBasePlayer", "m_bDucking");
    NETVAR(m_bInDuckJump, int, "CBasePlayer", "m_bInDuckJump");
    NETVAR(m_flDucktime, float, "CBasePlayer", "m_flDucktime");
    NETVAR(m_flDuckJumpTime, float, "CBasePlayer", "m_flDuckJumpTime");
    NETVAR(m_flJumpTime, float, "CBasePlayer", "m_flJumpTime");
    NETVAR(m_flFallVelocity, float, "CBasePlayer", "m_flFallVelocity");
    NETVAR(m_vecPunchAngle, vec3, "CBasePlayer", "m_vecPunchAngle");
    NETVAR(m_vecPunchAngleVel, vec3, "CBasePlayer", "m_vecPunchAngleVel");
    NETVAR(m_bDrawViewmodel, int, "CBasePlayer", "m_bDrawViewmodel");
    NETVAR(m_bWearingSuit, int, "CBasePlayer", "m_bWearingSuit");
    NETVAR(m_bPoisoned, int, "CBasePlayer", "m_bPoisoned");
    NETVAR(m_bForceLocalPlayerDraw, int, "CBasePlayer", "m_bForceLocalPlayerDraw");
    NETVAR(m_flStepSize, float, "CBasePlayer", "m_flStepSize");
    NETVAR(m_bAllowAutoMovement, int, "CBasePlayer", "m_bAllowAutoMovement");
    NETVAR(m_szScriptOverlayMaterial, char*, "CBasePlayer", "m_szScriptOverlayMaterial");
    NETVAR(m_Local, RecvTable*, "CBasePlayer", "m_Local");
    NETVAR(m_vecViewOffset, vec3, "CBasePlayer", "m_vecViewOffset[0]");
    NETVAR(m_flFriction, float, "CBasePlayer", "m_flFriction");
    NETVAR(m_iAmmo, RecvTable*, "CBasePlayer", "m_iAmmo");
    NETVAR(m_fOnTarget, int, "CBasePlayer", "m_fOnTarget");
    NETVAR(m_nTickBase, int, "CBasePlayer", "m_nTickBase");
    NETVAR(m_nNextThinkTick, int, "CBasePlayer", "m_nNextThinkTick");
    NETVAR(m_hLastWeapon, int, "CBasePlayer", "m_hLastWeapon");
    NETVAR(m_hGroundEntity, int, "CBasePlayer", "m_hGroundEntity");
    NETVAR(m_vecVelocity, vec3, "CBasePlayer", "m_vecVelocity[0]");
    NETVAR(m_vecBaseVelocity, vec3, "CBasePlayer", "m_vecBaseVelocity");
    NETVAR(m_hConstraintEntity, int, "CBasePlayer", "m_hConstraintEntity");
    NETVAR(m_vecConstraintCenter, vec3, "CBasePlayer", "m_vecConstraintCenter");
    NETVAR(m_flConstraintRadius, float, "CBasePlayer", "m_flConstraintRadius");
    NETVAR(m_flConstraintWidth, float, "CBasePlayer", "m_flConstraintWidth");
    NETVAR(m_flConstraintSpeedFactor, float, "CBasePlayer", "m_flConstraintSpeedFactor");
    NETVAR(m_flDeathTime, float, "CBasePlayer", "m_flDeathTime");
    NETVAR(m_nWaterLevel, int, "CBasePlayer", "m_nWaterLevel");
    NETVAR(m_flLaggedMovementValue, float, "CBasePlayer", "m_flLaggedMovementValue");
    NETVAR(localdata, RecvTable*, "CBasePlayer", "localdata");
    NETVAR(deadflag, int, "CBasePlayer", "deadflag");
    NETVAR(pl, RecvTable*, "CBasePlayer", "pl");
    NETVAR(m_iFOV, int, "CBasePlayer", "m_iFOV");
    NETVAR(m_iFOVStart, int, "CBasePlayer", "m_iFOVStart");
    NETVAR(m_flFOVTime, float, "CBasePlayer", "m_flFOVTime");
    NETVAR(m_iDefaultFOV, int, "CBasePlayer", "m_iDefaultFOV");
    NETVAR(m_hZoomOwner, int, "CBasePlayer", "m_hZoomOwner");
    NETVAR(m_hVehicle, int, "CBasePlayer", "m_hVehicle");
    NETVAR(m_hUseEntity, int, "CBasePlayer", "m_hUseEntity");
    NETVAR(m_iHealth, int, "CBasePlayer", "m_iHealth");
    NETVAR(m_lifeState, int, "CBasePlayer", "m_lifeState");
    NETVAR(m_iBonusProgress, int, "CBasePlayer", "m_iBonusProgress");
    NETVAR(m_iBonusChallenge, int, "CBasePlayer", "m_iBonusChallenge");
    NETVAR(m_flMaxspeed, float, "CBasePlayer", "m_flMaxspeed");
    NETVAR(m_fFlags, int, "CBasePlayer", "m_fFlags");
    NETVAR(m_iObserverMode, int, "CBasePlayer", "m_iObserverMode");
    NETVAR(m_hObserverTarget, int, "CBasePlayer", "m_hObserverTarget");
    NETVAR(m_hViewModel, int, "CBasePlayer", "m_hViewModel[0]");
    //NETVAR(m_hViewModel, array, "CBasePlayer", "m_hViewModel");
    NETVAR(m_szLastPlaceName, char*, "CBasePlayer", "m_szLastPlaceName");
};

class CBaseFlex {
public:
    NETVAR(m_flexWeight, RecvTable*, "CBaseFlex", "m_flexWeight");
    NETVAR(m_blinktoggle, int, "CBaseFlex", "m_blinktoggle");
    NETVAR(m_viewtarget, vec3, "CBaseFlex", "m_viewtarget");
};

class CBaseEntity : public IClientEntity {
public:
    NETVAR(m_flAnimTime, int, "CBaseEntity", "m_flAnimTime");
    NETVAR(AnimTimeMustBeFirst, RecvTable*, "CBaseEntity", "AnimTimeMustBeFirst");
    NETVAR(m_flSimulationTime, int, "CBaseEntity", "m_flSimulationTime");
    NETVAR(m_ubInterpolationFrame, int, "CBaseEntity", "m_ubInterpolationFrame");
    NETVAR(m_vecOrigin, vec3, "CBaseEntity", "m_vecOrigin");
    NETVAR(m_angRotation, vec3, "CBaseEntity", "m_angRotation");
    NETVAR(m_nModelIndex, int, "CBaseEntity", "m_nModelIndex");
    NETVAR(m_fEffects, int, "CBaseEntity", "m_fEffects");
    NETVAR(m_nRenderMode, int, "CBaseEntity", "m_nRenderMode");
    NETVAR(m_nRenderFX, int, "CBaseEntity", "m_nRenderFX");
    NETVAR(m_clrRender, int, "CBaseEntity", "m_clrRender");
    NETVAR(m_iTeamNum, int, "CBaseEntity", "m_iTeamNum");
    NETVAR(m_CollisionGroup, int, "CBaseEntity", "m_CollisionGroup");
    NETVAR(m_flElasticity, float, "CBaseEntity", "m_flElasticity");
    NETVAR(m_flShadowCastDistance, float, "CBaseEntity", "m_flShadowCastDistance");
    NETVAR(m_hOwnerEntity, int, "CBaseEntity", "m_hOwnerEntity");
    NETVAR(m_hEffectEntity, int, "CBaseEntity", "m_hEffectEntity");
    NETVAR(moveparent, int, "CBaseEntity", "moveparent");
    NETVAR(m_iParentAttachment, int, "CBaseEntity", "m_iParentAttachment");
    NETVAR(movetype, int, "CBaseEntity", "movetype");
    NETVAR(movecollide, int, "CBaseEntity", "movecollide");
    NETVAR(m_vecMinsPreScaled, vec3, "CBaseEntity", "m_vecMinsPreScaled");
    NETVAR(m_vecMaxsPreScaled, vec3, "CBaseEntity", "m_vecMaxsPreScaled");
    NETVAR(m_vecMins, vec3, "CBaseEntity", "m_vecMins");
    NETVAR(m_vecMaxs, vec3, "CBaseEntity", "m_vecMaxs");
    NETVAR(m_nSolidType, int, "CBaseEntity", "m_nSolidType");
    NETVAR(m_usSolidFlags, int, "CBaseEntity", "m_usSolidFlags");
    NETVAR(m_nSurroundType, int, "CBaseEntity", "m_nSurroundType");
    NETVAR(m_triggerBloat, int, "CBaseEntity", "m_triggerBloat");
    NETVAR(m_bUniformTriggerBloat, int, "CBaseEntity", "m_bUniformTriggerBloat");
    NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMinsPreScaled");
    NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxsPreScaled");
    NETVAR(m_vecSpecifiedSurroundingMins, vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMins");
    NETVAR(m_vecSpecifiedSurroundingMaxs, vec3, "CBaseEntity", "m_vecSpecifiedSurroundingMaxs");
    NETVAR(m_Collision, RecvTable*, "CBaseEntity", "m_Collision");
    NETVAR(m_iTextureFrameIndex, int, "CBaseEntity", "m_iTextureFrameIndex");
    NETVAR(m_PredictableID, int, "CBaseEntity", "m_PredictableID");
    NETVAR(m_bIsPlayerSimulated, int, "CBaseEntity", "m_bIsPlayerSimulated");
    NETVAR(predictable_id, RecvTable*, "CBaseEntity", "predictable_id");
    NETVAR(m_bSimulatedEveryTick, int, "CBaseEntity", "m_bSimulatedEveryTick");
    NETVAR(m_bAnimatedEveryTick, int, "CBaseEntity", "m_bAnimatedEveryTick");
    NETVAR(m_bAlternateSorting, int, "CBaseEntity", "m_bAlternateSorting");

    bool IsPlayer() {
        return GetClientClass()->m_ClassID == 27;
    }

    bool IsC4() {
        return GetClientClass()->m_ClassID == 23;
    }
};

class CBaseDoor {
public:
    NETVAR(m_flWaveHeight, float, "CBaseDoor", "m_flWaveHeight");
};

class CBaseCombatCharacter {
public:
    NETVAR(m_flNextAttack, float, "CBaseCombatCharacter", "m_flNextAttack");
    NETVAR(bcc_localdata, RecvTable*, "CBaseCombatCharacter", "bcc_localdata");
    NETVAR(m_hActiveWeapon, int, "CBaseCombatCharacter", "m_hActiveWeapon");
    NETVAR(m_hMyWeapons, RecvTable*, "CBaseCombatCharacter", "m_hMyWeapons");
};

class CBaseAnimatingOverlay {
public:
    NETVAR(m_AnimOverlay, RecvTable*, "CBaseAnimatingOverlay", "m_AnimOverlay");
    NETVAR(overlay_vars, RecvTable*, "CBaseAnimatingOverlay", "overlay_vars");
};

class CBoneFollower {
public:
    NETVAR(m_modelIndex, int, "CBoneFollower", "m_modelIndex");
    NETVAR(m_solidIndex, int, "CBoneFollower", "m_solidIndex");
};

class CBaseAnimating {
public:
    NETVAR(m_nSequence, int, "CBaseAnimating", "m_nSequence");
    NETVAR(m_nForceBone, int, "CBaseAnimating", "m_nForceBone");
    NETVAR(m_vecForce, vec3, "CBaseAnimating", "m_vecForce");
    NETVAR(m_nSkin, int, "CBaseAnimating", "m_nSkin");
    NETVAR(m_nBody, int, "CBaseAnimating", "m_nBody");
    NETVAR(m_nHitboxSet, int, "CBaseAnimating", "m_nHitboxSet");
    NETVAR(m_flModelScale, float, "CBaseAnimating", "m_flModelScale");
    NETVAR(m_flModelWidthScale, float, "CBaseAnimating", "m_flModelWidthScale");
    NETVAR(m_flPoseParameter, RecvTable*, "CBaseAnimating", "m_flPoseParameter");
    NETVAR(m_flPlaybackRate, float, "CBaseAnimating", "m_flPlaybackRate");
    NETVAR(m_flEncodedController, RecvTable*, "CBaseAnimating", "m_flEncodedController");
    NETVAR(m_bClientSideAnimation, int, "CBaseAnimating", "m_bClientSideAnimation");
    NETVAR(m_bClientSideFrameReset, int, "CBaseAnimating", "m_bClientSideFrameReset");
    NETVAR(m_nNewSequenceParity, int, "CBaseAnimating", "m_nNewSequenceParity");
    NETVAR(m_nResetEventsParity, int, "CBaseAnimating", "m_nResetEventsParity");
    NETVAR(m_nMuzzleFlashParity, int, "CBaseAnimating", "m_nMuzzleFlashParity");
    NETVAR(m_hLightingOrigin, int, "CBaseAnimating", "m_hLightingOrigin");
    NETVAR(m_hLightingOriginRelative, int, "CBaseAnimating", "m_hLightingOriginRelative");
    NETVAR(m_flCycle, float, "CBaseAnimating", "m_flCycle");
    NETVAR(serveranimdata, RecvTable*, "CBaseAnimating", "serveranimdata");
    NETVAR(m_fadeMinDist, float, "CBaseAnimating", "m_fadeMinDist");
    NETVAR(m_fadeMaxDist, float, "CBaseAnimating", "m_fadeMaxDist");
    NETVAR(m_flFadeScale, float, "CBaseAnimating", "m_flFadeScale");
};

class CInfoLightingRelative {
public:
    NETVAR(m_hLightingLandmark, int, "CInfoLightingRelative", "m_hLightingLandmark");
};

class CAI_BaseNPC {
public:
    NETVAR(m_lifeState, int, "CAI_BaseNPC", "m_lifeState");
    NETVAR(m_bPerformAvoidance, int, "CAI_BaseNPC", "m_bPerformAvoidance");
    NETVAR(m_bIsMoving, int, "CAI_BaseNPC", "m_bIsMoving");
    NETVAR(m_bFadeCorpse, int, "CAI_BaseNPC", "m_bFadeCorpse");
    NETVAR(m_iDeathPose, int, "CAI_BaseNPC", "m_iDeathPose");
    NETVAR(m_iDeathFrame, int, "CAI_BaseNPC", "m_iDeathFrame");
    NETVAR(m_iSpeedModRadius, int, "CAI_BaseNPC", "m_iSpeedModRadius");
    NETVAR(m_iSpeedModSpeed, int, "CAI_BaseNPC", "m_iSpeedModSpeed");
    NETVAR(m_bSpeedModActive, int, "CAI_BaseNPC", "m_bSpeedModActive");
    NETVAR(m_bImportanRagdoll, int, "CAI_BaseNPC", "m_bImportanRagdoll");
    NETVAR(m_flTimePingEffect, float, "CAI_BaseNPC", "m_flTimePingEffect");
};

class CBeam {
public:
    NETVAR(m_nBeamType, int, "CBeam", "m_nBeamType");
    NETVAR(m_nBeamFlags, int, "CBeam", "m_nBeamFlags");
    NETVAR(m_nNumBeamEnts, int, "CBeam", "m_nNumBeamEnts");
    NETVAR(m_hAttachEntity, RecvTable*, "CBeam", "m_hAttachEntity");
    NETVAR(m_nAttachIndex, RecvTable*, "CBeam", "m_nAttachIndex");
    NETVAR(m_nHaloIndex, int, "CBeam", "m_nHaloIndex");
    NETVAR(m_fHaloScale, float, "CBeam", "m_fHaloScale");
    NETVAR(m_fWidth, float, "CBeam", "m_fWidth");
    NETVAR(m_fEndWidth, float, "CBeam", "m_fEndWidth");
    NETVAR(m_fFadeLength, float, "CBeam", "m_fFadeLength");
    NETVAR(m_fAmplitude, float, "CBeam", "m_fAmplitude");
    NETVAR(m_fStartFrame, float, "CBeam", "m_fStartFrame");
    NETVAR(m_fSpeed, float, "CBeam", "m_fSpeed");
    NETVAR(m_flFrameRate, float, "CBeam", "m_flFrameRate");
    NETVAR(m_flHDRColorScale, float, "CBeam", "m_flHDRColorScale");
    NETVAR(m_clrRender, int, "CBeam", "m_clrRender");
    NETVAR(m_nRenderFX, int, "CBeam", "m_nRenderFX");
    NETVAR(m_nRenderMode, int, "CBeam", "m_nRenderMode");
    NETVAR(m_flFrame, float, "CBeam", "m_flFrame");
    NETVAR(m_vecEndPos, vec3, "CBeam", "m_vecEndPos");
    NETVAR(m_nModelIndex, int, "CBeam", "m_nModelIndex");
    NETVAR(m_nMinDXLevel, int, "CBeam", "m_nMinDXLevel");
    NETVAR(m_vecOrigin, vec3, "CBeam", "m_vecOrigin");
    NETVAR(moveparent, int, "CBeam", "moveparent");
    NETVAR(m_PredictableID, int, "CBeam", "m_PredictableID");
    NETVAR(m_bIsPlayerSimulated, int, "CBeam", "m_bIsPlayerSimulated");
    NETVAR(beampredictable_id, RecvTable*, "CBeam", "beampredictable_id");
};

class CBaseViewModel {
public:
    NETVAR(m_nModelIndex, int, "CBaseViewModel", "m_nModelIndex");
    NETVAR(m_nSkin, int, "CBaseViewModel", "m_nSkin");
    NETVAR(m_nBody, int, "CBaseViewModel", "m_nBody");
    NETVAR(m_nSequence, int, "CBaseViewModel", "m_nSequence");
    NETVAR(m_nViewModelIndex, int, "CBaseViewModel", "m_nViewModelIndex");
    NETVAR(m_flPlaybackRate, float, "CBaseViewModel", "m_flPlaybackRate");
    NETVAR(m_fEffects, int, "CBaseViewModel", "m_fEffects");
    NETVAR(m_nAnimationParity, int, "CBaseViewModel", "m_nAnimationParity");
    NETVAR(m_hWeapon, int, "CBaseViewModel", "m_hWeapon");
    NETVAR(m_hOwner, int, "CBaseViewModel", "m_hOwner");
    NETVAR(m_nNewSequenceParity, int, "CBaseViewModel", "m_nNewSequenceParity");
    NETVAR(m_nResetEventsParity, int, "CBaseViewModel", "m_nResetEventsParity");
    NETVAR(m_nMuzzleFlashParity, int, "CBaseViewModel", "m_nMuzzleFlashParity");
    NETVAR(m_flPoseParameter, float, "CBaseViewModel", "m_flPoseParameter[0]");
    //NETVAR(m_flPoseParameter, array, "CBaseViewModel", "m_flPoseParameter");
};

class CBaseProjectile {
public:
    NETVAR(m_hOriginalLauncher, int, "CBaseProjectile", "m_hOriginalLauncher");
};

class CBaseGrenade {
public:
    NETVAR(m_flDamage, float, "CBaseGrenade", "m_flDamage");
    NETVAR(m_DmgRadius, float, "CBaseGrenade", "m_DmgRadius");
    NETVAR(m_bIsLive, int, "CBaseGrenade", "m_bIsLive");
    NETVAR(m_hThrower, int, "CBaseGrenade", "m_hThrower");
    NETVAR(m_vecVelocity, vec3, "CBaseGrenade", "m_vecVelocity");
    NETVAR(m_fFlags, int, "CBaseGrenade", "m_fFlags");
};

class CBaseCombatWeapon {
public:
    NETVAR(m_iClip1, int, "CBaseCombatWeapon", "m_iClip1");
    NETVAR(m_iClip2, int, "CBaseCombatWeapon", "m_iClip2");
    NETVAR(m_iPrimaryAmmoType, int, "CBaseCombatWeapon", "m_iPrimaryAmmoType");
    NETVAR(m_iSecondaryAmmoType, int, "CBaseCombatWeapon", "m_iSecondaryAmmoType");
    NETVAR(m_nViewModelIndex, int, "CBaseCombatWeapon", "m_nViewModelIndex");
    NETVAR(m_nCustomViewmodelModelIndex, int, "CBaseCombatWeapon", "m_nCustomViewmodelModelIndex");
    NETVAR(m_bFlipViewModel, int, "CBaseCombatWeapon", "m_bFlipViewModel");
    NETVAR(LocalWeaponData, RecvTable*, "CBaseCombatWeapon", "LocalWeaponData");
    NETVAR(m_flNextPrimaryAttack, float, "CBaseCombatWeapon", "m_flNextPrimaryAttack");
    NETVAR(m_flNextSecondaryAttack, float, "CBaseCombatWeapon", "m_flNextSecondaryAttack");
    NETVAR(m_nNextThinkTick, int, "CBaseCombatWeapon", "m_nNextThinkTick");
    NETVAR(m_flTimeWeaponIdle, float, "CBaseCombatWeapon", "m_flTimeWeaponIdle");
    NETVAR(LocalActiveWeaponData, RecvTable*, "CBaseCombatWeapon", "LocalActiveWeaponData");
    NETVAR(m_iViewModelIndex, int, "CBaseCombatWeapon", "m_iViewModelIndex");
    NETVAR(m_iWorldModelIndex, int, "CBaseCombatWeapon", "m_iWorldModelIndex");
    NETVAR(m_iState, int, "CBaseCombatWeapon", "m_iState");
    NETVAR(m_hOwner, int, "CBaseCombatWeapon", "m_hOwner");
};

class Player_t : public CCSPlayer, public CBaseEntity {
public:

};
