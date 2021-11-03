#include "Graphic.h"
#include "CmnFunc.h"
#include "d3dcompiler.h"

namespace wrl = Microsoft::WRL;

Graphic::Graphic(HWND hWnd,int nWndWidth, int nWndHeight)
{
	mWnd = hWnd;
	mWndWidth = nWndWidth;
	mWndHeight = nWndHeight;

	mEnable4xMsaa = false;		
	m4xMsaaQuality = 0;

	mDepthStencilBuffer = nullptr;
	mRenderTargetView = nullptr;
	mDepthStencilView = nullptr;
	ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));
}

Graphic::~Graphic()
{
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);

	// Restore all default settings.
	if (mDeviceContext.Get() != nullptr)
		mDeviceContext->ClearState();

}

bool Graphic::Init()
{
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_9_1;

	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&mDevice,
		&featureLevel,
		&mDeviceContext);
	
	if( FAILED(hr) )
	{
		MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.
	HR(mDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mWndWidth;
	sd.BufferDesc.Height = mWndHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// Use 4X MSAA? 
	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	wrl::ComPtr<IDXGIDevice> dxgiDevice;
	HR(mDevice.Get()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	wrl::ComPtr<IDXGIAdapter> dxgiAdapter;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	wrl::ComPtr<IDXGIFactory> dxgiFactory;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(mDevice.Get(), &sd, &mSwapChain));

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.
	OnResize();

	return true;
}

void Graphic::OnResize()
{

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);

	// Resize the swap chain and recreate the render target view.
	HR(mSwapChain->ResizeBuffers(1, mWndWidth, mWndHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));				//改变交换链缓冲区的设置
	wrl::ComPtr<ID3D11Texture2D> backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));								//装备用ID3D11Texture2D接口来操作后向缓冲区backbuffer
	HR(mDevice->CreateRenderTargetView(backBuffer.Get(), 0, &mRenderTargetView));						//为读入的资源数据创建一个渲染目标视图

	//// Create the depth/stencil buffer and view.
	//D3D11_TEXTURE2D_DESC depthStencilDesc;																 //描述一个2D图片
	//depthStencilDesc.Width = mWndWidth;
	//depthStencilDesc.Height = mWndHeight;
	//depthStencilDesc.MipLevels = 1;
	//depthStencilDesc.ArraySize = 1;
	//depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//// Use 4X MSAA? --must match swap chain MSAA values.
	//if (mEnable4xMsaa)
	//{
	//	depthStencilDesc.SampleDesc.Count = 4;
	//	depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	//}
	//// No MSAA
	//else
	//{
	//	depthStencilDesc.SampleDesc.Count = 1;
	//	depthStencilDesc.SampleDesc.Quality = 0;
	//}
	//depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//depthStencilDesc.CPUAccessFlags = 0;
	//depthStencilDesc.MiscFlags = 0;

	//HR(mDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	//HR(mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));
	//// Bind the render target view and depth/stencil view to the pipeline.
	//mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// Set the viewport transform.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mWndWidth);
	mScreenViewport.Height = static_cast<float>(mWndHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mDeviceContext->RSSetViewports(1, &mScreenViewport);
}

void Graphic::UpdateScene(float dt)
{
	float value = sin(dt)*0.5 + 0.5;
	//float ClearColor[4] = { 0.5f, 0.1f, 0.2f, 1.0f }; //red,green,blue,alpha
	float ClearColor[4] = { value, value, 0.2f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, ClearColor);
}

void Graphic::DrawScene()
{
	DrawTriangle();
	mSwapChain->Present(1, 0);
}

void Graphic::DrawTriangle()
{
	struct Vertex
	{
		float x;
		float y;
	};

	Vertex vertexArr[] = {
		{0,0},{0,1},{1,0}
	};

	wrl::ComPtr<ID3D11Buffer> vertexBuff;

	D3D11_BUFFER_DESC vertexBufDesc;
	vertexBufDesc.ByteWidth = sizeof(Vertex) * 3;
	vertexBufDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vertexInitData;
	vertexInitData.pSysMem = vertexArr;
	HR(mDevice->CreateBuffer(&vertexBufDesc, &vertexInitData, &vertexBuff));

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &vertexBuff, &stride, &offset);
	
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	wrl::ComPtr<ID3D10Blob> compiledShader;										//ID3D10Blob用于返回任意长度的数据
	wrl::ComPtr<ID3D10Blob> compilationMsgs;									//错误信息
	D3DCompileFromFile(L"VertexShader.cso", 0, 0, 0, "fx_5_0", shaderFlags, 0, &compiledShader, &compilationMsgs);
	//D3DX11CreateEffectFromMemory()
	mDeviceContext->DrawIndexed(3, 0, 0);
}

