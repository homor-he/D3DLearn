#include "Graphic.h"
#include "CmnFunc.h"
#include "d3dcompiler.h"
#include <sstream>
#include "GraphicsThrowMacros.h"
#include "math.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Viewport.h"

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

	//应用程序调用com库函数之前必须初始化com库，为了使用DirectXTex
#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize))
		// error
#else
	//mCoInitialhr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	mCoInitialHr = CoInitialize(nullptr);
	if (FAILED(mCoInitialHr))
	{
		OutPutError("%s:%d\nFailed to CoInitialize\n *Error string:\n%s",
			__FILE__, __LINE__, TranslateErrorCode(mCoInitialHr).c_str());
	}
#endif
}

Graphic::~Graphic()
{
	//ReleaseCOM(mDepthStencilView);
	//ReleaseCOM(mDepthStencilBuffer);

	//恢复所有默认设定
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
	//检测是否支持抗锯齿
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
	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
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

	wrl::ComPtr<ID3D11Texture2D> backBuffer;
	GFX_THROW_INFO(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));								//装备用ID3D11Texture2D接口来操作后向缓冲区backbuffer
	mRenderTarget = shared_ptr<Bind::RenderTarget>(new Bind::OutputOnlyRenderTarget(*this, backBuffer.Get(), mEnable4xMsaa));//创建一个渲染目标视图
	mDepthStencil = make_shared<Bind::OutputOnlyDepthStencil>(*this, mWndWidth, mWndHeight, mEnable4xMsaa, m4xMsaaQuality);
	//mRenderTarget->BindAsBuffer(*this, mDepthStencil.get());														//绑定一个或更多渲染目标和深度模板缓冲区到管线的输出合并器阶段 

	//设置视口
	Bind::Viewport mScreenViewport(*this);
	mScreenViewport.Bind(*this);

	return true;
}

void Graphic::OnResize()
{
}

void Graphic::BeginFrame()
{
	/*float value = sin(dt)*0.5f + 0.5f;
	float ClearColor[4] = { value, value, 0.2f,1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTarget.Get(), ClearColor);*/
	mRenderTarget->Clear(*this);
	mDepthStencil->Clear(*this);
	//清除shader input资源，防止上一帧的PSSetShaderResources输入和下一帧的OMSetRenderTargets输出同时绑定，引发报错
	ID3D11ShaderResourceView * pNullTex = nullptr;
	mDeviceContext->PSSetShaderResources(0, 1, &pNullTex);
	mDeviceContext->PSSetShaderResources(3, 1, &pNullTex);
}

void Graphic::EndFrame()
{
	GFX_THROW_INFO_ONLY(mSwapChain->Present(1, 0));
}

int Graphic::GetWidth()
{
	return mWndWidth;
}

int Graphic::GetHeight()
{
	return mWndHeight;
}

void Graphic::DrawIndex(UINT count)
{
	GFX_THROW_INFO_ONLY(mDeviceContext->DrawIndexed(count, 0u, 0u));
}

void Graphic::SetCamera(DirectX::XMMATRIX & view)
{
	mCamera = view;
}

DirectX::XMMATRIX Graphic::GetCamera()
{
	return mCamera;
}

void Graphic::SetProjection(DirectX::FXMMATRIX & proj)
{
	mProj = proj;
}

DirectX::XMMATRIX Graphic::GetProjection()
{
	return mProj;
}

shared_ptr<Bind::RenderTarget> Graphic::GetRenderTarget()
{
	return mRenderTarget;
}

shared_ptr<Bind::DepthStencil> Graphic::GetDepthStencil()
{
	return mDepthStencil;
}

shared_ptr<Rgph::RenderGraph> Graphic::GetRenderGraph()
{
	if (!mRenderGraph)
		OutPutError("%s:%d\nrenderGraph is null", __FILE__, __LINE__);
	return mRenderGraph;
}

bool Graphic::Get4xMsaaStat()
{
	return mEnable4xMsaa;
}

UINT Graphic::GetMsaaQuality()
{
	return m4xMsaaQuality;
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
