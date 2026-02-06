#include "keyvalues.h"



auto KeyValues::LoadFromBuffer(const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID) -> bool{
	typedef bool(__thiscall* LoadFromBufferFn)(void*, const char*, const char*, void*, const char*);

	static LoadFromBufferFn pLoadFromBuffer = nullptr;
	if (!pLoadFromBuffer) {
		pLoadFromBuffer = (LoadFromBufferFn)(GetModuleHandleA("client.dll") + 0x2AF4F0);
		if (!pLoadFromBuffer) return false;
	}

	return pLoadFromBuffer(this, resourceName, pBuffer, pFileSystem, pPathID);
}