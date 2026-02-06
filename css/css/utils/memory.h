#pragma once

#define IsInRange( x, a, b ) ( x >= a && x <= b )
#define GetBits( x ) ( IsInRange( x, '0', '9' ) ? ( x - '0' ) : ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) )
#define GetByte( x ) ( GetBits( x[ 0 ] ) << 4 | GetBits( x[ 1 ] ) )

namespace memory {
    template<typename T>
    inline T GetInterface(const char* dllname, const char* interfacename) {
        auto hModule = GetModuleHandleA(dllname);
        if (!hModule) return nullptr;

        auto CreateInterface = (void*(*)(const char*, DWORD*))(GetProcAddress(hModule, "CreateInterface"));
        if (!CreateInterface) return nullptr;

        auto ret = reinterpret_cast<T>(CreateInterface(interfacename, nullptr));

        printf("[%s] from [%s] is [0x%llx]\n", interfacename, dllname, ret);

        return ret;
    }

    inline void**& GetVtable(void* inst) {
        return *reinterpret_cast<void***>(inst);
    }

    template<typename T>
    inline T GetVFunc(void* inst, const unsigned int index) {
        return reinterpret_cast<T>(GetVtable(inst));
    }

	std::uintptr_t PatternScan(const char* szMod, const char* szPattern)
	{
		auto hMod = GetModuleHandleA(szMod);

		if (!hMod)
			return 0;

		auto pDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>((std::uintptr_t)hMod);

		if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
			return 0;

		auto pNtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>((std::uintptr_t)hMod + pDosHeader->e_lfanew);

		if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
			return 0;

		auto pScanStart = reinterpret_cast<uint8_t*>((std::uintptr_t)hMod);
		auto pScanEnd = reinterpret_cast<uint8_t*>(pScanStart + pNtHeaders->OptionalHeader.SizeOfImage);

		std::uint8_t* pFirstMatch = nullptr;
		std::uint8_t* pByteArray = (std::uint8_t*)(szPattern);

		for (auto pCurrent = pScanStart; pCurrent < pScanEnd; pCurrent++)
		{
			if (*(BYTE*)pByteArray == (BYTE)'\?' || *pCurrent == GetByte(pByteArray))
			{
				if (!pFirstMatch)
					pFirstMatch = pCurrent;

				if (!pByteArray[2])
					return (std::uintptr_t)pFirstMatch;

				pByteArray += (*(std::uint16_t*)pByteArray == (std::uint16_t)'\?\?' || *(std::uint8_t*)pByteArray != (std::uint8_t)'\?') ? 3 : 2;

				if (!*pByteArray)
					return (std::uintptr_t)pFirstMatch;
			}
			else if (pFirstMatch)
			{
				pCurrent = pFirstMatch;
				pByteArray = (std::uint8_t*)szPattern;
				pFirstMatch = nullptr;
			}
		}
		return 0;
	}
}
