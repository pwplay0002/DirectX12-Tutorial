#pragma once

#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include "d3dx12.h"
#include "framework.h"

class D3DUtil
{

};

class com_exception : public std::exception
{
public:
	com_exception(HRESULT hr) : m_result(hr) { this->what(); }

	const char* what() const override
	{
		static char s_str[64] = {};
		sprintf_s(s_str, "Failure with HRESULT of %08X",
			static_cast<unsigned int>(m_result));
		return s_str;
	}

private:
	HRESULT m_result;
};

#ifndef ThrowIfFailed			
#define ThrowIfFailed(hr)			\
{									\
	HRESULT result = hr;			\
	if(FAILED(result))				\
	{								\
		throw com_exception(result);\
	}								\
}	
#endif // !ThrowIfFailed

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif