#pragma once
#include <memory>
#include <iostream>
#include <stdexcept>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class RenderSystem;
class GraphicEngine;
class Resource;
class ResourceManager;
class Texture;
class Texture2D;
class TextureManager;
class Mesh;
class MeshManager;
class Material;
class MainWindow;
class Game;
class Display;
class InputSystem;
class World;
class Entity;
class Component;
class TransformComponent;
class MeshComponent;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Texture2D> Texture2DPtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Material> MaterialPtr;

typedef unsigned int ui32;
typedef int i32;
typedef float f32;
typedef double d64;

enum class Key
{
	A = 0,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	Escape,
	Shift,
	Space,
	Enter,
	LeftMouseButton,
	MiddleMouseButton,
	RightMouseButton
};

#define Dx3DError(message)\
{\
std::wclog << "[DX3D Error] : " << message << std::endl;\
throw std::runtime_error("");\
}

#define Dx3DWarning(message)\
{\
std::wclog << "[DX3D Warning] : " << message << std::endl;\
}

#define Dx3DInfo(message)\
{\
std::wclog << "[DX3D Info] : " << message << std::endl;\
}