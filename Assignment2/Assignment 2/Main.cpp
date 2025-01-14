#include <cstdint>
#include <stdexcept>

#include <Windows.h>

#include "WindowHelper.h"
#include "D3D11Helper.h"
#include "ParallaxOcclusionPipeline.h"

void CreatePlane(ID3D11Device* device, RenderableObject& toInitialize, 
    float centerHeight)
{
    Vertex vertices[] =
    {
        { {-1.0f, centerHeight, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        { {1.0f, centerHeight, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        { {-1.0f, centerHeight, -1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        { {1.0f, centerHeight, -1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
    };
    toInitialize.vertexBuffer.Initialize(device, sizeof(Vertex), ARRAYSIZE(vertices),
        vertices);

    std::uint32_t indices[] = { 0, 1, 2, 2, 1, 3 };
    toInitialize.indexBuffer.Initialize(device, ARRAYSIZE(indices), indices);
}

void CreateCube(ID3D11Device* device, RenderableObject& toInitialize,
    float centerHeight)
{
    Vertex vertices[] =
    {
        // Front
        { {-1.0f, centerHeight + 1.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, // top left
            {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
        { {1.0f, centerHeight + 1.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, // top right
            {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
        { {-1.0f, centerHeight - 1.0f, -1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, // bottom left
            {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
        { {1.0f, centerHeight - 1.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, // bottom right
            {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },

        // Back
        { {1.0f, centerHeight + 1.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, // top left
            {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
        { {-1.0f, centerHeight + 1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, // top right
            {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
        { {1.0f, centerHeight - 1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, // bottom left
            {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
        { {-1.0f, centerHeight - 1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, // bottom right
            {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },

        // Left
        { {-1.0f, centerHeight + 1.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, // top left
            {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f} },
        { {-1.0f, centerHeight + 1.0f, -1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, // top right
            {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f} },
        { {-1.0f, centerHeight - 1.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, // bottom left
            {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f} },
        { {-1.0f, centerHeight - 1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, // bottom right
            {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f} },

        // right
        { {1.0f, centerHeight + 1.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // top left
            {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
        { {1.0f, centerHeight + 1.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // top right
            {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
        { {1.0f, centerHeight - 1.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, // bottom left
            {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
        { {1.0f, centerHeight - 1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, // bottom right
            {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} }

    };
    toInitialize.vertexBuffer.Initialize(device, sizeof(Vertex), ARRAYSIZE(vertices),
        vertices);

    std::uint32_t indices[] = { 
        0, 1, 2, 2, 1, 3,
        4, 5, 6, 6, 5, 7,
        8, 9, 10, 10, 9, 11,
        12, 13, 14, 14, 13, 15
    };
    toInitialize.indexBuffer.Initialize(device, ARRAYSIZE(indices), indices);
}

enum class ShapeType
{
    PLANE,
    CUBE
};

RenderableObject CreateRenderableObject(ID3D11Device* device,
    ShapeType typeOfShape, float centerHeight,
    const char* ambientMapPath, const char* diffuseMapPath, 
    const char* specularMapPath, const char* normalMapPath,
    const char* displacementMapPath)
{
    RenderableObject toReturn;

    switch (typeOfShape)
    {
    case ShapeType::PLANE:
        CreatePlane(device, toReturn, centerHeight);
        break;
    case ShapeType::CUBE:
        CreateCube(device, toReturn, centerHeight);
        break;
    default:
        throw std::runtime_error("Unknown shape type when creating renderable object");
    }

    Transform transform;
    DirectX::XMStoreFloat4x4(&transform.worldMatrix, DirectX::XMMatrixIdentity());
    toReturn.transformBuffer.Initialize(device, sizeof(Transform), &transform);

    toReturn.ambientMap.Initialize(device, ambientMapPath);
    toReturn.diffuseMap.Initialize(device, diffuseMapPath);
    toReturn.specularMap.Initialize(device, specularMapPath);
    toReturn.normalMap.Initialize(device, normalMapPath);
    toReturn.displacementMap.Initialize(device, displacementMapPath);

    return toReturn;
}

void UpdateTransforms(std::vector<RenderableObject>& objects,
    ID3D11DeviceContext* context, float amount)
{
    static float rotationAmount = 0.0f;
    rotationAmount += amount;
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(rotationAmount);
    DirectX::XMFLOAT4X4 toStore;
    DirectX::XMStoreFloat4x4(&toStore, rotationMatrix);

    for (RenderableObject& object : objects)
    {
        object.transformBuffer.UpdateBuffer(context, &toStore);
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	const UINT WINDOW_WIDTH = 1280;
	const UINT WINDOW_HEIGHT = 720;

	RenderWindow window = CreateRenderWindow(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, 
		L"Assignment 2", nullptr, L"Assignment 2", nCmdShow);
	GpuInterfaces interfaces = CreateGpuInterfaces();
	SwapChain swapChain = CreateSwapChain(interfaces.device, WINDOW_WIDTH,
		WINDOW_HEIGHT, window.handle);

    MSG msg = { };

    std::vector<RenderableObject> objects;
    objects.push_back(CreateRenderableObject(interfaces.device,
        ShapeType::PLANE, -1.25f, "..\\..\\Materials\\ambientMap.png", 
        "..\\..\\Materials\\diffuseMap.png", "..\\..\\Materials\\specularMap.png", 
        "..\\..\\Materials\\normalMap.png", "..\\..\\Materials\\displacementMap.png"));
    objects.push_back(CreateRenderableObject(interfaces.device,
        ShapeType::CUBE, 0.25f, "..\\..\\Materials\\ambientMap.png",
        "..\\..\\Materials\\diffuseMap.png", "..\\..\\Materials\\specularMap.png",
        "..\\..\\Materials\\normalMap.png", "..\\..\\Materials\\displacementMap.png"));

    ProjectionInfo projectionInfo;
    projectionInfo.fovAngleY = DirectX::XM_PIDIV4;
    projectionInfo.aspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    projectionInfo.nearZ = 0.01f;
    projectionInfo.farZ = 20.0f;
    CameraD3D11 camera;
    camera.Initialize(interfaces.device, projectionInfo, { 0.0f, 0.0f, -4.0f });

    SpotLightData::PerLightInfo lightInfo;
    lightInfo.colour = { 1.0f, 1.0f, 1.0f };
    lightInfo.rotationX = 0.0f;
    lightInfo.rotationY = 0.0f;
    lightInfo.angle = DirectX::XM_PIDIV4;
    lightInfo.projectionNearZ = 0.01f;
    lightInfo.projectionFarZ = 20.0f;
    lightInfo.initialPosition = { 0.0f, 0.0f, -5.0f };
    SpotLightData lightData;
    lightData.shadowMapInfo.textureDimension = 1;
    lightData.perLightInfo.push_back(lightInfo);
    SpotLightCollectionD3D11 lights;
    lights.Initialize(interfaces.device, lightData);

    DepthBufferD3D11 depthBuffer;
    depthBuffer.Initialize(interfaces.device, WINDOW_WIDTH, WINDOW_HEIGHT);

    ParallaxOcclusionPipeline pipeline;
    pipeline.Initialize(interfaces.device, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        camera.UpdateInternalConstantBuffer(interfaces.immediateContext);
        lights.UpdateLightBuffers(interfaces.immediateContext);
        UpdateTransforms(objects, interfaces.immediateContext, 0.005f);

        float clearColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        interfaces.immediateContext->ClearRenderTargetView(swapChain.rtv,
            clearColour);
        interfaces.immediateContext->ClearDepthStencilView(depthBuffer.GetDSV(0),
            D3D11_CLEAR_DEPTH, 1.0f, 0);

        pipeline.Render(objects, camera, lights, interfaces.immediateContext,
            swapChain.rtv, depthBuffer);

        swapChain.gpuInterface->Present(1, 0);
    }

    return 0;
}