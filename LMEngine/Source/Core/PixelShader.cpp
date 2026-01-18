#include <stdafx.h>
#include <PixelShader.h>
#include <RenderSystem.h>

PixelShader::PixelShader(const wchar_t* full_path, const char* entryPoint, RenderSystem* system) : m_ps(nullptr), m_system(system)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3DBlob> errBlob;

	D3DCompileFromFile(full_path, nullptr, nullptr, entryPoint, "ps_5_0", 0, 0, &blob, &errBlob);

	if (errBlob)
		Dx3DWarning("PixelShader" << full_path << " compiled with errors: \n" << (char*)errBlob->GetBufferPointer());

	if (!blob)
		Dx3DError("PixelShader" << full_path << " compilation failed.");

	if (FAILED(m_system->m_d3d_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_ps)))
		throw std::exception("[D3D11 Error] PixelShader creation failed.");

}