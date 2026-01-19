#include <stdafx.h>
#include <Core/VertexShader.h>
#include <Core/RenderSystem.h>

VertexShader::VertexShader(const wchar_t* full_path, const char* entryPoint, RenderSystem* system) : m_vs(nullptr), m_system(system)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3DBlob> errBlob;

	D3DCompileFromFile(full_path, nullptr, nullptr, entryPoint, "vs_5_0", 0, 0, &blob, &errBlob);

	if (errBlob)
		Dx3DWarning("VertexShader" << full_path << " compiled with errors: \n" << (char*)errBlob->GetBufferPointer());

	if (!blob)
		Dx3DError("VertexShader" << full_path << " compilation failed.");

	if (FAILED(m_system->m_d3d_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_vs)))
		Dx3DError("VertexShader creation failed.");
}
