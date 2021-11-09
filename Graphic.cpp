#include "Graphic.h"
#include "CmnFunc.h"
#include "d3dcompiler.h"
#include <sstream>
#include "GraphicsThrowMacros.h"

namespace wrl = Microsoft::WRL;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

Graphic::Graphic(HWND hWnd,int nWndWidth, int nWndHeight)
{
	mWnd = hWnd;
	mWndWidth = nWndWidth;
	mWndHeight = nWndHeight;

	mEnable4xMsaa = false;		
	m4xMsaaQuality = 0;

	//mDepthStencilBuffer = nullptr;
	//mDepthStencilView = nullptr;
	//ZeroMemory(&mScreenViewport, sizeof(D3D11_VIEWPORT));
}

Graphic::~Graphic()
{
	//ReleaseCOM(mDepthStencilView);
	//ReleaseCOM(mDepthStencilBuffer);

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
	GFX_THROW_INFO(mDevice->CheckMultisampleQualityLevels(
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

	wrl::ComPtr<IDXGIDevice> dxgiDevice;
	GFX_THROW_INFO(mDevice.Get()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	wrl::ComPtr<IDXGIAdapter> dxgiAdapter;
	GFX_THROW_INFO(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	wrl::ComPtr<IDXGIFactory> dxgiFactory;
	GFX_THROW_INFO(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	GFX_THROW_INFO(dxgiFactory->CreateSwapChain(mDevice.Get(), &sd, &mSwapChain));

	/*D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
		nullptr, 0, D3D11_SDK_VERSION, &sd, &mSwapChain, &mDevice, nullptr, &mDeviceContext);*/

	OnResize();

	return true;
}

void Graphic::OnResize()
{

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	//ReleaseCOM(mDepthStencilView);
	//ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(mRenderTarget);

	HRESULT hr;
	// Resize the swap chain and recreate the render target view.
	GFX_THROW_INFO(mSwapChain->ResizeBuffers(1, mWndWidth, mWndHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));				//改变交换链缓冲区的设置
	wrl::ComPtr<ID3D11Texture2D> backBuffer;
	GFX_THROW_INFO(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));								//装备用ID3D11Texture2D接口来操作后向缓冲区backbuffer
	GFX_THROW_INFO(mDevice->CreateRenderTargetView(backBuffer.Get(), 0, &mRenderTarget));						//为读入的资源数据创建一个渲染目标视图

	//设置渲染目标
	mDeviceContext->OMSetRenderTargets(1, &mRenderTarget, 0);
	
	//设置视口
	D3D11_VIEWPORT mScreenViewport;
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
	float value = sin(dt)*0.5f + 0.5f;
	//float ClearColor[4] = { 0.5f, 0.1f, 0.2f, 1.0f }; //red,green,blue,alpha
	float ClearColor[4] = { value, value, 0.2f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTarget, ClearColor);
}

void Graphic::DrawScene()
{
	DrawTriangle();
	GFX_THROW_INFO_ONLY(mSwapChain->Present(1, 0));
}

void Graphic::DrawTriangle()
{
	// for checking results of d3d functions
	HRESULT hr;
	struct Vertex
	{
		typedef struct
		{
			float x;
			float y;
		}Pos;
		typedef struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		}Color;
		Pos pos;
		Color color;
	};

	Vertex vertexArr[] = {
		
		/*{0.0f,0.5f,255,0,0,255},
		{0.5f,-0.5f,0,255,0,255},
		{-0.5f,-0.5f,0,0,255,255},

		{0.0f,0.5f,255,0,0,255},
		{ 0.5f,0.0f,0,255,0,255 },
		{ 0.5f,-0.5f,0,0,255,255 },

		{-0.5f,-0.5f,0,0,255,255},
		{ 0.5f,-0.5f,0,255,0,255 },
		{ 0.0f,-0.75f,0,255,0,255 },

		{ 0.0f,0.5f,255,0,0,255 },
		{ -0.5f,-0.5f,0,0,255,255 },
		{ -0.5f,0.0f,0,0,255,255 },*/

		{ 0.0f,0.5f,255,0,0,255 },
		{ 0.5f,-0.5f,0,255,0,255 },
		{ -0.5f,-0.5f,0,0,255,255 },
		{ 0.5f,0.0f,0,255,0,255 },
		{ 0.0f,-0.75f,0,255,0,255 },
		{ -0.5f,0.0f,0,0,255,255 },
	};

	//创建顶点缓存
	wrl::ComPtr<ID3D11Buffer> vertexBuff;
	D3D11_BUFFER_DESC vertexBufDesc;
	vertexBufDesc.ByteWidth = sizeof(vertexArr);
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA vertexInitData;
	vertexInitData.pSysMem = vertexArr;
	GFX_THROW_INFO(mDevice->CreateBuffer(&vertexBufDesc, &vertexInitData, &vertexBuff));

	SHORT indexArr[] =
	{
		0,1,2,
		1,0,3,
		1,4,2,
		0,2,5,
	};
	//创建索引缓存
	wrl::ComPtr<ID3D11Buffer> indexBuff;
	D3D11_BUFFER_DESC indexBufDesc;
	indexBufDesc.ByteWidth = sizeof(indexArr);
	indexBufDesc.Usage = D3D11_USAGE_IMMUTABLE;;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = indexArr;
	GFX_THROW_INFO(mDevice->CreateBuffer(&indexBufDesc, &indexInitData, &indexBuff));
	//设置顶点缓存
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, vertexBuff.GetAddressOf(), &stride, &offset);
	//设置索引缓存
	mDeviceContext->IASetIndexBuffer(indexBuff.Get(), DXGI_FORMAT_R16_UINT, 0);

	//读取顶点着色器
	wrl::ComPtr<ID3DBlob> vsBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &vsBlob));
	//创建并设置顶点着色器
	wrl::ComPtr<ID3D11VertexShader> vertexShader;
	GFX_THROW_INFO(mDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &vertexShader));
	mDeviceContext->VSSetShader(vertexShader.Get(), 0, 0);

	//读取像素着色器
	wrl::ComPtr<ID3DBlob> psBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &psBlob));
	//创建并设置像素着色器
	wrl::ComPtr<ID3D11PixelShader>  pixelShader;
	mDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &pixelShader);
	mDeviceContext->PSSetShader(pixelShader.Get(), 0, 0);

	//设置inputlayout
	wrl::ComPtr<ID3D11InputLayout> inputLayout;
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA ,0},
		{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8,D3D11_INPUT_PER_VERTEX_DATA ,0 },
	};
	mDevice->CreateInputLayout(vertexDesc, size(vertexDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
	mDeviceContext->IASetInputLayout(inputLayout.Get());

	//设置几何图元形状
	mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GFX_THROW_INFO_ONLY(mDeviceContext->DrawIndexed(size(indexArr), 0u, 0u));
}

Graphic::HrException::HrException(int line, const char * file, HRESULT hr, vector<string> infoMsgs) 
	: ExceptionBase(line, file)
{
	this->hr = hr;
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char * Graphic::HrException::what() const 
{
	ostringstream sOut;
	sOut << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		sOut << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	sOut << GetOriginString();
	whatBuffer = sOut.str();
	return whatBuffer.c_str();
}

const char * Graphic::HrException::GetType() const
{
	return "Graphics Exception";
}

HRESULT Graphic::HrException::GetErrorCode() const
{
	return hr;
}

std::string Graphic::HrException::GetErrorString() const
{
	return DXGetErrorString(hr);
}

std::string Graphic::HrException::GetErrorDescription() const
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphic::HrException::GetErrorInfo() const
{
	return info;
}

Graphic::InfoException::InfoException(int line, const char * file, std::vector<std::string> infoMsgs) 
	: ExceptionBase(line,file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
	
}

const char * Graphic::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char * Graphic::InfoException::GetType() const
{
	return "Graphic InfoException";
}

string Graphic::InfoException::GetErrorInfo() const
{
	return info;
}
