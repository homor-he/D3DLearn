//***************************************************************************************
// d3dApp.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "d3dApp.h"
#include <WindowsX.h>
#include <sstream>
#include "Mouse.h"
#include "MathHelper.h"
#include "CmnFunc.h" 
#include "TechChannels.h"
#include "ShadowRenderGraph.h"

#include "FirstCase.h"
#include "TextureCase.h"
#include "GeometryCase.h"
#include "SeveralGeometryCase.h"
#include "CloneCase.h"
#include "LightCase.h"
#include "GeometryInLightCase.h"
#include "GeometryCaseWithTech.h"
#include "ReadObjCase.h"
#include "ReadObjWithModel.h"
#include "SkyboxCase.h"
#include "PlaneCase.h"
#include "WaveCase.h"
#include "TestRenderGraph.h"
#include "Surface.h"

D3DApp * gd3dApp = NULL;

DrawTest::FirstTest m_firstCase;
DrawTest::TextureCase m_textureCase;
DrawTest::GeometryCase m_geometryCase;
DrawTest::SeveralGeometryCase m_severalGeometryCase;
DrawTest::CloneCase m_cloneCase;
DrawTest::GeometryInLightCase m_geometryInLightCase;
DrawTest::GeometryCaseWithTech m_geometryCaseWithTech;
DrawTest::GeometryCaseWithTech m_geometryCaseWithTech2;
DrawTest::ReadObjCase m_readObjCase;
DrawTest::ReadObjWithModel m_readObjWithModel;
DrawTest::SkyboxCase m_skyBoxCase;
DrawTest::PointLightCase m_pointLightCase;
DrawTest::SpotLightCase m_spotLightCase;
DrawTest::DirectLightCase m_dirLightCase;
DrawTest::PlaneCase m_planeCase;
DrawTest::PlaneCase m_planeCase2;
DrawTest::WaveCase m_waveCase;

D3DApp::D3DApp()
{
	// Get a pointer to the application object so we can forward 
	// Windows messages to the object's window procedure through
	// the global window procedure.
	gd3dApp = this;
	
}

D3DApp::~D3DApp()
{
}

HINSTANCE D3DApp::Inst()const
{
	return mWindow->Inst();
}

shared_ptr<Window> D3DApp::GetWindow() const
{
	return mWindow;
}

bool D3DApp::InitWnd(HINSTANCE hInstance, int nWndWidth, int nWndHeight, string szWndCaption)
{
	mWindow = make_shared<Window>(hInstance, szWndCaption, nWndWidth, nWndHeight);
	if (mWindow != nullptr)
		mWindow->InitWindow();
	Graphic & gfx = *mWindow->GetGraphic();
	mCamera = make_shared<Camera>(*mWindow->GetGraphic(), "Camera", mWindow->AspectRatio());
	mCamera->BindToGraphic(gfx);
	//要渲染的内容初始化
	//m_severalGeometryCase.Init(*this);
	//m_cloneCase.Init(*this);
	//m_readObjCase.Init(*this);
	//m_geometryInLightCase.Init(*this);
	//m_skyBoxCase.Init(gfx);
	m_dirLightCase.Init(gfx);
	m_spotLightCase.Init(gfx);
	m_pointLightCase.Init(gfx);
	m_waveCase.Init(gfx);
	m_planeCase.Init(gfx);
	m_planeCase2.Init(gfx);
	m_geometryCaseWithTech.Init(gfx);
	m_geometryCaseWithTech2.Init(gfx);
	m_readObjWithModel.Init(gfx);
	

	//渲染pass集合，用于实现各种想要的效果
	//mRenderGraph = make_shared<Rgph::TestRenderGraph>(*this);
	mRenderGraph = make_shared<Rgph::ShadowRenderGraph>(gfx);
	mRenderGraph->BindMainCamera(*mCamera);
	mRenderGraph->BindShadowCamera(*m_pointLightCase.ShareCamera());

	//将渲染物体本身的step和pass通道联系起来
	m_planeCase.LinkTechnique(*mRenderGraph);
	m_planeCase.Submit(Chan::main);
	m_planeCase.Submit(Chan::shadow);

	m_planeCase2.LinkTechnique(*mRenderGraph);
	m_planeCase2.Submit(Chan::main);
	m_planeCase2.Submit(Chan::shadow);
	m_planeCase2.m_plane->SetPos(DirectX::XMFLOAT3(40.0f, 40.0f, 0.0f));
	m_planeCase2.m_plane->SetRotation(0.0f, 0.0f, DirectX::XM_PI*0.5f);

	m_geometryCaseWithTech.LinkTechnique(*mRenderGraph);
	m_geometryCaseWithTech.Submit(Chan::main);
	m_geometryCaseWithTech.Submit(Chan::shadow);
	m_geometryCaseWithTech.m_cube->SetPos(DirectX::XMFLOAT3(0.0f, 5.0f, -10.0f));

	m_geometryCaseWithTech2.LinkTechnique(*mRenderGraph);
	m_geometryCaseWithTech2.Submit(Chan::main);
	m_geometryCaseWithTech2.Submit(Chan::shadow);
	m_geometryCaseWithTech2.m_cube->SetPos(DirectX::XMFLOAT3(0.0f, 5.0f, 10.0f));

	m_readObjWithModel.LinkTechnique(*mRenderGraph);
	m_readObjWithModel.Submit(Chan::main);
	m_readObjWithModel.Submit(Chan::shadow);

	m_waveCase.LinkTechnique(*mRenderGraph);
	m_waveCase.Submit(Chan::main);
	m_waveCase.Submit(Chan::shadow);

	Surface surface = Surface::FromFile("images\\0cfa2d4253998ba38b345952a941bb7f.jpeg");
	Surface::Color* pColor = surface.GetBufferPtr();
	UINT size = surface.GetBufferSize()/4;
	for (int i = 0; i < size; ++i)
	{
		WriteLog("color: x:%d r:%d g:%d b:%d",pColor->GetX(),pColor->GetR(),pColor->GetG(),pColor->GetB());
		pColor++;
	}
	return true;
}

void D3DApp::DoFrame(float dt)
{
	Graphic & gfx = *mWindow->GetGraphic();
	gfx.BeginFrame();
	mCamera->SetLocationAndRotation();
	//mRenderGraph->BindMainCamera(*m_pointLightCase.ShareCamera());

	//天空盒
	//m_skyBoxCase.Draw(*this, dt);
	//平行光
	//m_dirLightCase.Draw(*this, dt);
	//m_dirLightCase.Render(*this);
	//聚光灯
	//m_spotLightCase.Draw(*this, dt);
	//m_spotLightCase.Render(*this);
	//点光源
	DirectX::XMFLOAT4 camWorldPos = mCamera->GetWorldPos();
	m_pointLightCase.Draw(gfx, dt);
	m_pointLightCase.Render(gfx, DirectX::XMFLOAT3(camWorldPos.x, camWorldPos.y, camWorldPos.z));

	//m_severalGeometryCase.Draw(*this, dt);
	//m_cloneCase.Draw(*this, dt);
	//m_geometryInLightCase.Draw(*this, dt);
	//m_readObjCase.Draw(*this, dt);

	//m_planeCase.Draw(gfx, dt);

	//m_geometryCaseWithTech.Draw(gfx, dt);
	//m_readObjWithModel.Draw(gfx, dt);

	m_waveCase.UpdateWave(gfx, mTimer.TotalTime(), dt);

	mRenderGraph->Execute(gfx);
	
	gfx.EndFrame();

	//if (!mSave)
	//{
		//mRenderGraph->DumpShadowMap(gfx, "shadow");
		//mSave = true;
	//}
}

int D3DApp::Run()
{
	MSG msg = {0};

	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			mTimer.Tick();
			if( !mWindow->mAppPaused)
			{
				//计算帧率
				CalculateFrameStats();
				//处理输入
				HandleInput();
				//更新绘制窗口
				DoFrame(mTimer.GetFixTime());
			}
			else
			{
				Sleep(100);
			}
        }
    }

	return (int)msg.wParam;
}

void D3DApp::CalculateFrameStats()
{
	static int sFrameCnt = 0;
	static float sStaticTime = mTimer.TotalTime();

	sFrameCnt++;
	mTimer.TotalFrameIncrement();

	if (mTimer.TotalTime() - sStaticTime >= 1.0f)
	{
		float perFrameTime = 1.0f / sFrameCnt;
		char outputStr[1024] = { 0 };
		sprintf_s(outputStr, "帧率:%d fps,每帧:%.3f", sFrameCnt, perFrameTime);
		SetWindowText(mWindow->Wnd(), outputStr);
		sFrameCnt = 0;
		sStaticTime += 1.0f;
	}
}

void D3DApp::HandleInput()
{
	//鼠标输入事件响应
	Mouse::Event sMouseEvent(Mouse::Event::Type::Default, mWindow->mMouse);
	if (mWindow->mMouse.ReadEventBuffer(sMouseEvent))
	{
		switch (sMouseEvent.GetType())
		{
			case Mouse::Event::Type::LPress:
			{
				mWindow->mMouse.m_lastMousePosX = sMouseEvent.GetXPos();
				mWindow->mMouse.m_lastMousePosY = sMouseEvent.GetYPos();
				break;
			}
			case Mouse::Event::Type::Move:
			{
				if (mWindow->mMouse.leftIsPressed)
				{
					float dx = DirectX::XMConvertToRadians(static_cast<float>(sMouseEvent.GetXPos() - mWindow->mMouse.m_lastMousePosX));
					float dy = DirectX::XMConvertToRadians(static_cast<float>(sMouseEvent.GetYPos() - mWindow->mMouse.m_lastMousePosY));
					//防止鼠标抖动时，屏幕抖动
					if (abs(dx) < 0.01f && abs(dy) < 0.01f)
						return;
					//变化值较大的一个方向移动，防止横纵坐标抖动
					if (abs(dx) < abs(dy))
					{
						//纵向移动
						mCamera->m_homePitch += (dy >= 0.0f) ? 0.02f : -0.02f;
						mCamera->m_homePitch = MathHelper::Clamp(mCamera->m_homePitch, -MathHelper::Pi / 2.0f, MathHelper::Pi / 2.0f);
					}
					else
					{
						//横向移动
						mCamera->m_homeYaw += (dx >= 0.0f) ? 0.02f : -0.02f;
					}
				}
				mWindow->mMouse.m_lastMousePosX = sMouseEvent.GetXPos();
				mWindow->mMouse.m_lastMousePosY = sMouseEvent.GetYPos();
				break;
			}
			case Mouse::Event::Type::Leave:
			{
				mWindow->mMouse.leftIsPressed = false;
				mWindow->mMouse.rightIsPressed = false;
				break;
			}
			default:
				break;
		}
	}

	//键盘输入事件响应
	Keyboard::Event sKbEvent(Keyboard::Event::Type::Default, ' ');
	if (mWindow->mKeyboard.ReadKey(sKbEvent))
	{
		switch (sKbEvent.GetType())
		{
			case Keyboard::Event::Type::Press:
			{
				switch (sKbEvent.GetCode())
				{
					case 'W':
						mCamera->SetMoveVector(0.0f, 0.0f, 0.3f);
						break;
					case 'S':
						mCamera->SetMoveVector(0.0f, 0.0f, -0.3f);
						break;
					case 'A':
						mCamera->SetMoveVector(-0.3f, 0.0f, 0.0f);
						break;
					case 'D':
						mCamera->SetMoveVector(0.3f, 0.0f, 0.0f);
						break;
					default:
						break;
				}
				break;
			}
			case Keyboard::Event::Type::Release:
			{
				switch (sKbEvent.GetCode())
				{
				case 'W':
				case 'S':
				case 'A':
				case 'D':
					mCamera->SetMoveVector(0.0f, 0.0f, 0.0f);
					break;
				default:
					break;
				}
				break;
			}
			default:
				break;
		}
	}
}







