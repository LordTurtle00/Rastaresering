#include "CameraD3D11.h"

#include <array>

using namespace DirectX;

void CameraD3D11::MoveInDirection(float amount, const XMFLOAT3& direction)
{
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR dir = XMLoadFloat3(&direction);
	pos += (dir * amount);
	XMStoreFloat3(&position, pos);
}

void CameraD3D11::RotateAroundAxis(float amount, const XMFLOAT3& axis)
{
	XMMATRIX rotationMatrix = XMMatrixRotationNormal({ axis.x, axis.y, axis.z }, amount);
	std::array<XMFLOAT3*, 3> vectorsToRotate = { &forward, &right, &up };

	for (XMFLOAT3* vector : vectorsToRotate)
	{
		if (&axis == vector)
			continue;

		XMVECTOR temp = { vector->x, vector->y, vector->z };
		XMStoreFloat3(vector, XMVector3Transform(temp, rotationMatrix));
	}
}

CameraD3D11::CameraD3D11(ID3D11Device* device, const ProjectionInfo& projectionInfo,
	const XMFLOAT3& initialPosition)
{
	Initialize(device, projectionInfo, initialPosition);
}

void CameraD3D11::Initialize(ID3D11Device* device, const ProjectionInfo& projectionInfo,
	const XMFLOAT3& initialPosition)
{
	position = initialPosition;
	projInfo = projectionInfo;
	cameraBuffer.Initialize(device, sizeof(CameraDataCB));
}

void CameraD3D11::MoveForward(float amount)
{
	MoveInDirection(amount, forward);
}

void CameraD3D11::MoveRight(float amount)
{
	MoveInDirection(amount, right);
}

void CameraD3D11::MoveUp(float amount)
{
	MoveInDirection(amount, up);
}

void CameraD3D11::RotateForward(float amount)
{
	RotateAroundAxis(amount, forward);
}

void CameraD3D11::RotateRight(float amount)
{
	RotateAroundAxis(amount, right);
}

void CameraD3D11::RotateUp(float amount)
{
	RotateAroundAxis(amount, up);
}

const DirectX::XMFLOAT3& CameraD3D11::GetPosition() const
{
	return position;
}

const DirectX::XMFLOAT3& CameraD3D11::GetForward() const
{
	return forward;
}

const DirectX::XMFLOAT3& CameraD3D11::GetRight() const
{
	return right;
}

const DirectX::XMFLOAT3& CameraD3D11::GetUp() const
{
	return up;
}

void CameraD3D11::UpdateInternalConstantBuffer(ID3D11DeviceContext* context)
{
	CameraDataCB toUpload;
	toUpload.vp = GetViewProjectionMatrix();
	toUpload.position = position;
	cameraBuffer.UpdateBuffer(context, &toUpload);
}

ID3D11Buffer* CameraD3D11::GetConstantBuffer() const
{
	return cameraBuffer.GetBuffer();
}

DirectX::XMFLOAT4X4 CameraD3D11::GetViewProjectionMatrix() const
{
	XMVECTOR eyePos = XMLoadFloat3(&position);
	XMVECTOR lookAt = eyePos + XMLoadFloat3(&forward);
	XMMATRIX viewMatrix = XMMatrixLookAtLH(eyePos, lookAt, { up.x, up.y, up.z });

	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(projInfo.fovAngleY,
		projInfo.aspectRatio, projInfo.nearZ, projInfo.farZ);

	XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, XMMatrixTranspose(viewMatrix * projectionMatrix)); 
	
	return data;
}
