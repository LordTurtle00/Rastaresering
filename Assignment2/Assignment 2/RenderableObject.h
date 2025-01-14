#pragma once

#include <DirectXMath.h>

#include "VertexBufferD3D11.h"
#include "IndexBufferD3D11.h"
#include "ConstantBufferD3D11.h"
#include "ShaderResourceTextureD3D11.h"

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 textureCoords;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 bitangent;
};

struct Transform
{
	DirectX::XMFLOAT4X4 worldMatrix;
};

struct RenderableObject
{
	VertexBufferD3D11 vertexBuffer;
	IndexBufferD3D11 indexBuffer;
	ConstantBufferD3D11 transformBuffer;
	ShaderResourceTextureD3D11 ambientMap;
	ShaderResourceTextureD3D11 diffuseMap;
	ShaderResourceTextureD3D11 specularMap;
	ShaderResourceTextureD3D11 normalMap;
	ShaderResourceTextureD3D11 displacementMap;
};