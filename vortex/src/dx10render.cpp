#include "vtx_dx10render.h"
#include "vtx_assertions.h"

using namespace platform;

DX10Render::DX10Render(RenderCreationParams &params, WINDOW outputWindow)
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	IDXGIFactory *dxfactory;
	ID3D10Texture2D *backBuffer;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxfactory);
	
	result = (D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &this->device));
	ASSERT(result == S_OK);

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = params.backBufferSize.x;
	swapChainDesc.BufferDesc.Height = params.backBufferSize.y;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO: RenderCreationParams enum!
	swapChainDesc.OutputWindow = outputWindow;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count = params.multisampleCount;
	swapChainDesc.SampleDesc.Quality = params.multisampleQuality;
	swapChainDesc.Windowed = TRUE;

	result = dxfactory->CreateSwapChain(this->device, &swapChainDesc, &this->swapChain);
	ASSERT(result == S_OK);

	result = this->swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**)&backBuffer);
	ASSERT(result == S_OK);

	result = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	ASSERT(result == S_OK);

	device->OMSetRenderTargets(1, &renderTargetView, NULL);

	D3D10_VIEWPORT vp;
    vp.Width = params.backBufferSize.x;
    vp.Height = params.backBufferSize.y;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    device->RSSetViewports( 1, &vp );
	
}

void DX10Render::clear()
{
	this->device->ClearRenderTargetView(this->renderTargetView, this->clearColor.values);
}

void DX10Render::swap(void)
{
	this->swapChain->Present(0, 0);
}