class CKeyValues {
public:	
	CKeyValues(const char* setName) {
		auto fn = (void(__thiscall*)(CKeyValues*, const char*))(GetModuleHandleA("client.dll") + 0x2AE940);
		fn(this, setName);
	}

	void* operator new(size_t n_alloc_size) {
		auto fn = (void*(__thiscall*)(size_t))(GetModuleHandleA("client.dll") + 0x2AE9A0);
		return fn(n_alloc_size);
	}

	bool LoadFromBuffer(const char* resourceName, const char* pBuffer, int pFileSystem = 0, const char* pPathID = nullptr) {
		auto fn = (bool(__thiscall*)(CKeyValues*, char*, char*, int, const char*))(GetModuleHandleA("client.dll") + 0x2AF4F0);
		return fn(this, _strdup(resourceName), _strdup(pBuffer), pFileSystem, pPathID);
	}

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