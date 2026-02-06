#pragma once
#include <cstdint>
#include <Windows.h>
#include "utils/Vectors.h"
#include "utils/memory.h"

class CBaseHandle;
class IClientRenderable;
class CStudioHdr;
class Ray_t;
class IWorldRenderList;
class VPlane;
class VisibleFogVolumeInfo_t;
class ITexture;
struct model_t;
struct dlight_t;
struct WorldListInfo_t;
struct VisOverrideData_t;
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
class CSentence;
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
    ClientClass(char* pNetworkName, CreateClientClassFn createFn, CreateEventFn createEventFn, void* pRecvTable) {
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
    void* m_pRecvTable;
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
    virtual void release(void) = 0;
    virtual const vec3& get_abs_origin(void) const = 0;
    virtual const vec3& get_abs_angles(void) const = 0;
    virtual void* get_mouth(void) = 0;
    virtual bool get_sound_spatialization(int& info) = 0;

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
    /*1*/ virtual void* __destuctor__() = 0;
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
    int      x;
    int      m_nUnscaledX;
    int      y;
    int      m_nUnscaledY;
    int      width;
    int      m_nUnscaledWidth;
    int      height;
    int      m_eStereoEye;
    int      m_nUnscaledHeight;
    bool    m_bOrtho;
    float    m_OrthoLeft;
    float    m_OrthoTop;
    float    m_OrthoRight;
    float    m_OrthoBottom;
    float    fov;
    float    fovViewmodel;
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

class CPanel{
public:
    /*0*/ virtual void* GetVPanel(void) = 0;
    /*1*/ virtual void* Think(void) = 0;
    /*2*/ virtual void* PerformApplySchemeSettings(void) = 0;
    /*3*/ virtual void* PaintTraverse(bool, bool) = 0;
    /*4*/ virtual void* Repaint(void) = 0;
    /*5*/ virtual void* IsWithinTraverse(int, int, bool) = 0;
    /*6*/ virtual void* GetInset(int&, int&, int&, int&) = 0;
    /*7*/ virtual void* GetClipRect(int&, int&, int&, int&) = 0;
    /*8*/ virtual void* OnChildAdded(unsigned int) = 0;
    /*9*/ virtual void* OnSizeChanged(int, int) = 0;
    /*10*/ virtual void* InternalFocusChanged(bool) = 0;
    /*11*/ virtual void* RequestInfo(CKeyValues*) = 0;
    /*12*/ virtual void* RequestFocus(int) = 0;
    /*13*/ virtual void* RequestFocusPrev(unsigned int) = 0;
    /*14*/ virtual void* RequestFocusNext(unsigned int) = 0;
    /*15*/ virtual void* OnMessage(CKeyValues const*, unsigned int) = 0;
    /*16*/ virtual void* GetCurrentKeyFocus(void) = 0;
    /*17*/ virtual void* GetTabPosition(void) = 0;
    /*18*/ virtual void* GetName(void) = 0;
    /*19*/ virtual void* GetClassName__(void) = 0;
    /*20*/ virtual void* GetScheme(void) = 0;
    /*21*/ virtual void* IsProportional(void) = 0;
    /*22*/ virtual void* IsAutoDeleteSet(void) = 0;
    /*23*/ virtual void* DeletePanel(void) = 0;
    /*24*/ virtual void* QueryInterface(int) = 0;
    /*25*/ virtual void* GetPanel(void) = 0;
    /*26*/ virtual void* GetModuleName(void) = 0;
    /*27*/ virtual void* OnTick(void) = 0;
    /*28*/ virtual void* GetMessageMap(void) = 0;
    /*29*/ virtual void* GetAnimMap(void) = 0;
    /*30*/ virtual void* GetKBMap(void) = 0;
};

class Entity_t : public IClientEntity {
public:
    bool IsPlayer() {
        return GetClientClass()->m_ClassID == 27;
    }
    
    bool IsC4() {
        return GetClientClass()->m_ClassID == 23;
    }
};

class Player_t : public Entity_t {
public:

};


