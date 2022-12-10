#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#pragma once

#include "CmnHead.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <DirectXMath.h>
#include "ExceptionBase.h"
#include "DxgiInfoManager.h"
#include <memory>
#include "RenderGraph.h"

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

namespace Bind
{
	class RenderTarget;
	class DepthStencil;
}


class Graphic
{
	friend class GraphicResource;
public:
	class HrException : public ExceptionBase
	{
	public:
		HrException(int line, const char* file, HRESULT hr, vector<string> infoMsgs = {});
		const char* what() const override;
		const char* GetType() const override;
		HRESULT GetErrorCode() const;
		string GetErrorString() const;
		string GetErrorDescription() const;
		string GetErrorInfo() const;
	private:
		HRESULT hr;
		string info;
	};
	class InfoException : public ExceptionBase
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs);
		const char* what() const noexcept override;
		const char* GetType() const override;
		string GetErrorInfo() const;
	private:
		string info;
	};
	
public:
	Graphic(HWND hWnd, int nWndWidth, int nWndHeight);
	virtual ~Graphic();

	bool Init();
	void OnResize();
	void BeginFrame();
	void EndFrame();
	int GetWidth();
	int GetHeight();
	void DrawIndex(UINT count);
	void SetCamera(DirectX::XMMATRIX & view);
	DirectX::XMMATRIX GetCamera();
	void SetProjection(DirectX::FXMMATRIX & proj);
	DirectX::XMMATRIX GetProjection();
	shared_ptr<Bind::RenderTarget> GetRenderTarget();
	shared_ptr<Bind::DepthStencil> GetDepthStencil();
	shared_ptr<Rgph::RenderGraph> GetRenderGraph();
	bool Get4xMsaaStat();
	UINT GetMsaaQuality();
private:
	HWND      mWnd;
	int		  mWndWidth;
	int		  mWndHeight;

	DirectX::XMMATRIX mCamera;
	DirectX::XMMATRIX mProj;

	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	//ID3D11RenderTargetView* mRenderTarget;
	//Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTarget;
	//ID3D11Texture2D* mDepthStencilBuffer;
	//ID3D11DepthStencilView* mDepthStencilView;
	shared_ptr<Bind::RenderTarget>  mRenderTarget;
	shared_ptr<Bind::DepthStencil>  mDepthStencil;
	
	bool	  mEnable4xMsaa;		//ÊÇ·ñ¿ªÆô¿¹¾â³Ý
	UINT	  m4xMsaaQuality;
#if ((defined DEBUG) || (defined _DEBUG))
	DxgiInfoManager infoManager;
#endif
	HRESULT mCoInitialHr;

	shared_ptr<Rgph::RenderGraph> mRenderGraph;

	DWORD m_save = false;
};


#endif // !__GRAPHIC_H__
