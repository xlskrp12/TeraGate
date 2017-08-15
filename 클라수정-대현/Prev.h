#pragma once

#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

#define WIND32_LEAN_AND_MEAN
#pragma comment(lib, "winmm.lib")// playsound() ����� ���� ����
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
//#pragma comment(lib, "D3Dcompiler.lib")
// #pragma comment(lib, "D3Dcompiler.lib")
#include<time.h>        // Ÿ�̸� ����� ���� ����

//------------------------------------------------
#include<WinSock2.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include<WS2tcpip.h>
#include"..\..\TeraGateServer\protocol.h"

#include"Server.h"

#pragma comment(lib, "ws2_32")
#pragma comment(lib,"msimg32")
#pragma warning(disable:4996)
//------------------------------------------------

#include<windows.h>
#include<stdio.h>       // sprintf() ����� ���� ����
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<iostream>      // #include<fstream>, using std::ofstream ����� ���� ����
#include<fstream>       // unsing std::ofstream ����� ���� ����
#include<D3D11.h>       // #include<D3DX11.h> d3d11, d3dx �������� ����?;
#include<D3DX11.h>      // D3DX11CompileFromFile()�Լ� ��밡��. D3D11�� �ȵ�.
#include<D3Dcompiler.h> // ���̴� ������ �Լ� ����
#include<D3DX10math.h>  // Direct3D Math �Լ� ����
#include<D3DX11async.h> // CreateVertexShaderFromFile() �Լ� ����ε� �� �ƿ� �ȵ���..
#include<d3dcommon.h>
#include<DXGI.h>
// #include<DirectXMath.h> // XMFLOAT3 ����ü ��밡��..�ϱ� �ѵ� �׳� D3DX10math.h�� D3DXVECTOR3 ����ü ����.. ������ �����ε��� �̰����� ���ִ�.


const bool  FULL_SCREEN   = false;
const bool  VSYNC_ENABLED = true;
const float SCREEN_DEPTH  = 20000.0f; // �� �̻� �Ѿ�� �Ǹ� �˾Ƽ� �ø��ø� ><
const float SCREEN_NEAR   = 0.1f;
void infoD3DXMATRIX(D3DXMATRIX mtx);
void infoD3DXMATRIX(D3DXMATRIX mtx, char* desc);
void infoD3DXVECTOR3(D3DXVECTOR3 v);
void infoD3DXVECTOR3(D3DXVECTOR3 v, char* desc);
void ifFail(HRESULT hResult);

//------------------------------------------------
#define _ServerDef		Server::GetInstance()
#define _InGameDef		SceneIngame::GetInstance()
//------------------------------------------------