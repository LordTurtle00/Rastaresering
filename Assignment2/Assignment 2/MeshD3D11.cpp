#include "MeshD3D11.h"

void MeshD3D11::Initialize(ID3D11Device* device, const MeshData& meshInfo)
{
	vertexBuffer.Initialize(device, meshInfo.vertexInfo.sizeOfVertex,
		meshInfo.vertexInfo.nrOfVerticesInBuffer, meshInfo.vertexInfo.vertexData);

	indexBuffer.Initialize(device, meshInfo.indexInfo.nrOfIndicesInBuffer,
		meshInfo.indexInfo.indexData);

	for (const auto& subMeshInfo : meshInfo.subMeshInfo)
	{
		subMeshes.emplace_back();
		subMeshes.back().Initialize(subMeshInfo.startIndexValue, subMeshInfo.nrOfIndicesInSubMesh,
			subMeshInfo.ambientTextureSRV, subMeshInfo.diffuseTextureSRV, subMeshInfo.specularTextureSRV);
	}
}

void MeshD3D11::BindMeshBuffers(ID3D11DeviceContext* context) const
{
	auto vb = vertexBuffer.GetBuffer();
	auto stride = vertexBuffer.GetVertexSize();
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	auto ib = indexBuffer.GetBuffer();
	context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
}

void MeshD3D11::PerformSubMeshDrawCall(ID3D11DeviceContext* context, size_t subMeshIndex) const
{
	subMeshes[subMeshIndex].PerformDrawCall(context);
}

size_t MeshD3D11::GetNrOfSubMeshes() const
{
	return subMeshes.size();
}

ID3D11ShaderResourceView* MeshD3D11::GetAmbientSRV(size_t subMeshIndex) const
{
	return subMeshes[subMeshIndex].GetAmbientSRV();
}

ID3D11ShaderResourceView* MeshD3D11::GetDiffuseSRV(size_t subMeshIndex) const
{
	return subMeshes[subMeshIndex].GetDiffuseSRV();
}

ID3D11ShaderResourceView* MeshD3D11::GetSpecularSRV(size_t subMeshIndex) const
{
	return subMeshes[subMeshIndex].GetSpecularSRV();
}

void MeshD3D11::SetDiffuseSRV(ID3D11ShaderResourceView* srv, size_t submeshIndex)
{
	subMeshes[submeshIndex].SetDiffuseSRV(srv);
}
