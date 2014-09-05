//-----------------------------------------------------------------------------
// File: Vertices.cpp
//
// Desc: In this tutorial, we are rendering some Vertices. This introduces the
//       concept of the vertex buffer, a Direct3D object used to store
//       Vertices. Vertices can be defined any way we want by defining a
//       custom structure and a custom FVF (flexible vertex format). In this
//       tutorial, we are using Vertices that are transformed (meaning they
//       are already in 2D window coordinates) and lit (meaning we are not
//       using Direct3D lighting, but are supplying our own colors).
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <tchar.h>
#include <strsafe.h>
#pragma warning( default : 4996 )
//#include <Windows.h>
#include <mmsystem.h>
#include <math.h>
#include <dos.h>
#include <stdio.h>
#include <string>
#include <WinUser.h>
#include <WindowsX.h>
#include <conio.h>
#include <string.h>
#include <commdlg.h>
#include <ShlObj.h>
#include <UserEnv.h>
#pragma comment(lib, "Userenv.lib")
#include <objidl.h> 
#include <shlobj.h>
#include <string.h>
#include <Shlwapi.h>
#include "resource.h"
#include "Settings.h"
#include <atlstr.h>

#define HOTKEY(modifier,key) ((((modifier)&0xff)<<8)|((key)&0xff))

int WND_W = 200;
int WND_H = 200;
int RButton = 3;
int WindowVisIndex = 100;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB1 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB2 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB3 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB4 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB5 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB6 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB11 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB22 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB33 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB44 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB55 = NULL; // Buffer to hold Vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB66 = NULL; // Buffer to hold Vertices

LPD3DXRENDERTOSURFACE   pRenderToSurface = NULL;
LPDIRECT3DTEXTURE9      pDynamicTexture  = NULL;
LPDIRECT3DSURFACE9      pTextureSurface  = NULL;
LPDIRECT3DSURFACE9      offScreenSurface = NULL;

LPDIRECT3DTEXTURE9      g_pTexturesRight[20] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
										   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }; 
LPDIRECT3DTEXTURE9      g_pTexturesLeft[12] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }; 

BITMAPINFO BmpInfo;

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The transformed position for the vertex
    DWORD color;        // The vertex color
    FLOAT tu, tv;   // The texture coordinates
};

// Our custom FVF, which describes our custom vertex structure

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_SPECULAR | D3DFVF_TEX1 )

#define WM_SHELLNOTE (WM_USER + 1)

#define BUFSIZE 4096

#define ID_ADMINISTRATOR    1
#define ID_SETTINGS         2
#define ID_EXIT             3

HWND hWnd;
HDC hDC;
HMENU hMenu;

bool CheckedZ = FALSE;
bool CheckedX = FALSE;
bool CheckedY = FALSE;
bool AutorunChecked = FALSE;
bool TrayChecked = FALSE;
bool TaskbarChecked = TRUE;
bool Changing = TRUE;
bool SetS = false;
bool R[5] = {false, false, true, false, false};

DWORD dwStyle = WS_OVERLAPPEDWINDOW;
WORD Numbers = 0;

TCHAR szPathTextures[MAX_PATH]; 

SYSTEMTIME st;

int mm, hh;
float fAngleV1=0, fAngleV2=0, fAngleV3=0, fAngleV4=0, fAngleV5=0, fAngleV6=0;

int imin1, imin2, imin3;
int jmin1, jmin2, jmin3;

int ihours1, ihours2, ihours3;
int jhours1, jhours2, jhours3;

int Posx = 100, Posy = 100;

int Alpha=0;

int str=0;
BOOL ADDTA;
BOOL ADDTT;
BOOL SHOWONT;

HRESULT CreateDynamicTexture()
{
	HRESULT HR;

	HR = D3DXCreateTexture( g_pd3dDevice, 
		                    WND_W,
							WND_H,
							1, 
							D3DUSAGE_RENDERTARGET, 
							D3DFMT_A8R8G8B8, 
							D3DPOOL_DEFAULT, 
							&pDynamicTexture );

	if( FAILED(HR) )
	{
		return HR;
    }

	D3DSURFACE_DESC D3DDesc;
    pDynamicTexture->GetSurfaceLevel( 0, &pTextureSurface );
	
    pTextureSurface->GetDesc( &D3DDesc );

	HR = D3DXCreateRenderToSurface( g_pd3dDevice, 
		                            D3DDesc.Width, 
		                            D3DDesc.Height, 
		                            D3DDesc.Format, 
		                            TRUE, 
		                            D3DFMT_D16, 
		                            &pRenderToSurface);

	HR = g_pd3dDevice->CreateOffscreenPlainSurface(D3DDesc.Width, D3DDesc.Height, D3DDesc.Format, D3DPOOL_SYSTEMMEM, &offScreenSurface, NULL );


    if( FAILED(HR) )
    {
		return HR;
    }
}


//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D()
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling, so we see the front and back of the triangle
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	
	//-F- Enabling Z-buffer
	g_pd3dDevice->SetRenderState (D3DRS_ZENABLE, D3DZB_TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

    // Turn off D3D lighting, since we are providing our own vertex colors
    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Device state would normally be set here

	CreateDynamicTexture();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Creates a vertex buffer and fills it with our Vertices. The vertex
//       buffer is basically just a chuck of memory that holds Vertices. After
//       creating it, we must Lock()/Unlock() it to fill it. For indices, D3D
//       also uses index buffers. The special thing about vertex and index
//       buffers is that they can be created in device memory, allowing some
//       cards to process them in hardware, resulting in a dramatic
//       performance gain.
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
	TCHAR szPathR0[MAX_PATH],
		  szPathR1[MAX_PATH],
		  szPathR2[MAX_PATH],
		  szPathR3[MAX_PATH],
		  szPathR4[MAX_PATH],
		  szPathR5[MAX_PATH],
		  szPathR6[MAX_PATH],
		  szPathR7[MAX_PATH],
		  szPathR8[MAX_PATH],
		  szPathR9[MAX_PATH],
		  szPathR00[MAX_PATH],
		  szPathR11[MAX_PATH],
		  szPathR22[MAX_PATH],
		  szPathR33[MAX_PATH],
		  szPathR44[MAX_PATH],
		  szPathR55[MAX_PATH],
		  szPathR66[MAX_PATH],
		  szPathR77[MAX_PATH],
		  szPathR88[MAX_PATH],
		  szPathR99[MAX_PATH],
		  szPathL0[MAX_PATH],
		  szPathL1[MAX_PATH],
		  szPathL2[MAX_PATH],
		  szPathL3[MAX_PATH],
		  szPathL4[MAX_PATH],
		  szPathL5[MAX_PATH],
		  szPathL00[MAX_PATH],
		  szPathL11[MAX_PATH],
		  szPathL22[MAX_PATH],
		  szPathL33[MAX_PATH],
		  szPathL44[MAX_PATH],
		  szPathL55[MAX_PATH];

	if ( (_tcslen(szPathTextures))== 0 )
	{
		GetCurrentDirectory( sizeof(szPathTextures), szPathTextures );
		wcscat( szPathTextures,L"\\Textures\\Standart" );
	}

	_tcscpy( szPathR0, szPathTextures );
	_tcscpy( szPathR1, szPathTextures );
	_tcscpy( szPathR2, szPathTextures );
	_tcscpy( szPathR3, szPathTextures );
	_tcscpy( szPathR4, szPathTextures );
	_tcscpy( szPathR5, szPathTextures );
	_tcscpy( szPathR6, szPathTextures );
	_tcscpy( szPathR7, szPathTextures );
	_tcscpy( szPathR8, szPathTextures );
	_tcscpy( szPathR9, szPathTextures );
	_tcscpy( szPathR00, szPathTextures );
	_tcscpy( szPathR11, szPathTextures );
	_tcscpy( szPathR22, szPathTextures );
	_tcscpy( szPathR33, szPathTextures );
	_tcscpy( szPathR44, szPathTextures );
	_tcscpy( szPathR55, szPathTextures );
	_tcscpy( szPathR66, szPathTextures );
	_tcscpy( szPathR77, szPathTextures );
	_tcscpy( szPathR88, szPathTextures );
	_tcscpy( szPathR99, szPathTextures );
	_tcscpy( szPathL0, szPathTextures );
	_tcscpy( szPathL1, szPathTextures );
	_tcscpy( szPathL2, szPathTextures );
	_tcscpy( szPathL3, szPathTextures );
	_tcscpy( szPathL4, szPathTextures );
	_tcscpy( szPathL5, szPathTextures );
	_tcscpy( szPathL00, szPathTextures );
	_tcscpy( szPathL11, szPathTextures );
	_tcscpy( szPathL22, szPathTextures );
	_tcscpy( szPathL33, szPathTextures );
	_tcscpy( szPathL44, szPathTextures );
	_tcscpy( szPathL55, szPathTextures );

	wcscat( szPathR0, L"\\Right\\0.png" );
	wcscat( szPathR1, L"\\Right\\1.png" );
	wcscat( szPathR2, L"\\Right\\2.png" );
	wcscat( szPathR3, L"\\Right\\3.png" );
	wcscat( szPathR4, L"\\Right\\4.png" );
	wcscat( szPathR5, L"\\Right\\5.png" );
	wcscat( szPathR6, L"\\Right\\6.png" );
	wcscat( szPathR7, L"\\Right\\7.png" );
	wcscat( szPathR8, L"\\Right\\8.png" );
	wcscat( szPathR9, L"\\Right\\9.png" );
	wcscat( szPathR00, L"\\Right\\00.png" );
	wcscat( szPathR11, L"\\Right\\11.png" );
	wcscat( szPathR22, L"\\Right\\22.png" );
	wcscat( szPathR33, L"\\Right\\33.png" );
	wcscat( szPathR44, L"\\Right\\44.png" );
	wcscat( szPathR55, L"\\Right\\55.png" );
	wcscat( szPathR66, L"\\Right\\66.png" );
	wcscat( szPathR77, L"\\Right\\77.png" );
	wcscat( szPathR88, L"\\Right\\88.png" );
	wcscat( szPathR99, L"\\Right\\99.png" );
	wcscat( szPathL0, L"\\Left\\0.png" );
	wcscat( szPathL1, L"\\Left\\1.png" );
	wcscat( szPathL2, L"\\Left\\2.png" );
	wcscat( szPathL3, L"\\Left\\3.png" );
	wcscat( szPathL4, L"\\Left\\4.png" );
	wcscat( szPathL5, L"\\Left\\5.png" );
	wcscat( szPathL00, L"\\Left\\00.png" );
	wcscat( szPathL11, L"\\Left\\11.png" );
	wcscat( szPathL22, L"\\Left\\22.png" );
	wcscat( szPathL33, L"\\Left\\33.png" );
	wcscat( szPathL44, L"\\Left\\44.png" );
	wcscat( szPathL55, L"\\Left\\55.png" );

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR0, &g_pTexturesRight[0] ) ) )
	{
		MessageBox::Show( L"Could not find 0.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR1, &g_pTexturesRight[1] ) ) )
    {
		MessageBox::Show( L"Could not find 1.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR2, &g_pTexturesRight[2] ) ) )
	{
		MessageBox::Show( L"Could not find 2.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR3, &g_pTexturesRight[3] ) ) )
    {
		MessageBox::Show( L"Could not find 3.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR4, &g_pTexturesRight[4] ) ) )
	{
		MessageBox::Show( L"Could not find 4.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR5, &g_pTexturesRight[5] ) ) )
    {
		MessageBox::Show( L"Could not find 5.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR6, &g_pTexturesRight[6] ) ) )
	{
		MessageBox::Show( L"Could not find 6.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR7, &g_pTexturesRight[7] ) ) )
    {
		MessageBox::Show( L"Could not find 7.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }
	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR8, &g_pTexturesRight[8] ) ) )
	{
		MessageBox::Show( L"Could not find 8.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR9, &g_pTexturesRight[9] ) ) )
    {
		MessageBox::Show( L"Could not find 9.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR00, &g_pTexturesRight[10] ) ) )
	{
		MessageBox::Show( L"Could not find 00.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR11, &g_pTexturesRight[11] ) ) )
    {
		MessageBox::Show( L"Could not find 11.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR22, &g_pTexturesRight[12] ) ) )
	{
		MessageBox::Show( L"Could not find 22.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR33, &g_pTexturesRight[13] ) ) )
    {
		MessageBox::Show( L"Could not find 33.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR44, &g_pTexturesRight[14] ) ) )
	{
		MessageBox::Show( L"Could not find 44.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR55, &g_pTexturesRight[15] ) ) )
    {
		MessageBox::Show( L"Could not find 55.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR66, &g_pTexturesRight[16] ) ) )
	{
		MessageBox::Show( L"Could not find 66.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR77, &g_pTexturesRight[17] ) ) )
    {
		MessageBox::Show( L"Could not find 77.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }
	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR88, &g_pTexturesRight[18] ) ) )
	{
		MessageBox::Show( L"Could not find 88.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathR99, &g_pTexturesRight[19] ) ) )
    {
		MessageBox::Show( L"Could not find 99.png (Right)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL0, &g_pTexturesLeft[0] ) ) )
    {
		MessageBox::Show( L"Could not find 0.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL1, &g_pTexturesLeft[1] ) ) )
    {
		MessageBox::Show( L"Could not find 1.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL2, &g_pTexturesLeft[2] ) ) )
    {
		MessageBox::Show( L"Could not find 2.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL3, &g_pTexturesLeft[3] ) ) )
    {
		MessageBox::Show( L"Could not find 3.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL4, &g_pTexturesLeft[4] ) ) )
    {
		MessageBox::Show( L"Could not find 4.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL5, &g_pTexturesLeft[5] ) ) )
    {
		MessageBox::Show( L"Could not find 5.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL00, &g_pTexturesLeft[6] ) ) )
    {
		MessageBox::Show( L"Could not find 00.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL11, &g_pTexturesLeft[7] ) ) )
    {
		MessageBox::Show( L"Could not find 11.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL22, &g_pTexturesLeft[8] ) ) )
    {
		MessageBox::Show( L"Could not find 22.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL33, &g_pTexturesLeft[9] ) ) )
    {
		MessageBox::Show( L"Could not find 33.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL44, &g_pTexturesLeft[10] ) ) )
    {
		MessageBox::Show( L"Could not find 44.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, szPathL55, &g_pTexturesLeft[11] ) ) )
    {
		MessageBox::Show( L"Could not find 55.png (Left)", L"Textures.exe", MessageBoxButtons::OK, MessageBoxIcon::Error );
    }

	// Initialize three Vertices for rendering a triangle
    CUSTOMVERTEX Vertices1[] =
    {

		{ -1.23f, 0.60f, 0.0f, 0xffffffff, 0, 0, },
        { -0.63f, 0.00f, 0.0f, 0xff0000ff, 1, 0.5, },
        { -1.23f, 0.00f, 0.0f, 0xffff0000, 0, 0.5, },

		{ -0.63f, 0.00f, 0.0f, 0xff0000ff, 1, 0.5, },
        { -1.23f, 0.60f, 0.0f, 0xffffffff, 0, 0, },
        { -0.63f, 0.60f, 0.0f, 0xffff0000, 1, 0, }
	};

	CUSTOMVERTEX Vertices11[] =
    {

		{ -0.63f, 0.60f, 0.0f, 0xffffffff, 0, 0, },
        { -0.03f, 0.00f, 0.0f, 0xff0000ff, 1, 0.5, },
        { -0.63f, 0.00f, 0.0f, 0xffff0000, 0, 0.5, },

		{ -0.03f, 0.00f, 0.0f, 0xff0000ff, 1, 0.5, },
        { -0.63f, 0.60f, 0.0f, 0xffffffff, 0, 0, },
        { -0.03f, 0.60f, 0.0f, 0xffff0000, 1, 0, }
	};

    CUSTOMVERTEX Vertices2[] =
    {
		{ -1.23f, -0.60f, 0.0f, 0xffffffff, 0, 1, },
        { -0.63f, -0.00f, 0.0f, 0xff00ff00, 1, 0.5, },
        { -1.23f, -0.00f, 0.0f, 0xff800000, 0, 0.5, },

		{ -0.63f, -0.00f, 0.0f, 0xff00ff00, 1, 0.5, },
        { -1.23f, -0.60f, 0.0f, 0xffffffff, 0, 1, },
        { -0.63f, -0.60f, 0.0f, 0xff800000, 1, 1, }
	};

    CUSTOMVERTEX Vertices22[] =
    {
		{ -0.63f, -0.60f, 0.0f, 0xffffffff, 0, 1, },
        { -0.03f, -0.00f, 0.0f, 0xff00ff00, 1, 0.5, },
        { -0.63f, -0.00f, 0.0f, 0xff800000, 0, 0.5, },

		{ -0.03f, -0.00f, 0.0f, 0xff00ff00, 1, 0.5, },
        { -0.63f, -0.60f, 0.0f, 0xffffffff, 0, 1, },
        { -0.03f, -0.60f, 0.0f, 0xff800000, 1, 1, }
	};

	CUSTOMVERTEX Vertices3[] =
    {
		{  0.63f, 0.60f, 0.0f, 0xffffffff, 1, 0, },
        {  0.03f, 0.00f, 0.0f, 0xff0000ff, 0, 0.5, },
        {  0.63f, 0.00f, 0.0f, 0xffff0000, 1, 0.5, },

		{  0.03f, 0.00f, 0.0f, 0xff0000ff, 0, 0.5, },
        {  0.63f, 0.60f, 0.0f, 0xffffffff, 1, 0, },
        {  0.03f, 0.60f, 0.0f, 0xffff0000, 0, 0, }
	};

	CUSTOMVERTEX Vertices33[] =
    {
		{  1.23f, 0.60f, 0.0f, 0xffffffff, 1, 0, },
        {  0.63f, 0.00f, 0.0f, 0xff0000ff, 0, 0.5, },
        {  1.23f, 0.00f, 0.0f, 0xffff0000, 1, 0.5, },

		{  0.63f, 0.00f, 0.0f, 0xff0000ff, 0, 0.5, },
        {  1.23f, 0.60f, 0.0f, 0xffffffff, 1, 0, },
        {  0.63f, 0.60f, 0.0f, 0xffff0000, 0, 0, }
	};

	CUSTOMVERTEX Vertices4[] =
    {
		{  0.63f, -0.60f, 0.0f, 0xffffffff, 1, 1, },
        {  0.03f, -0.00f, 0.0f, 0xff00ff00, 0, 0.5, },
        {  0.63f, -0.00f, 0.0f, 0xff800000, 1, 0.5, },

		{  0.03f, -0.00f, 0.0f, 0xff00ff00, 0, 0.5, },
        {  0.63f, -0.60f, 0.0f, 0xffffffff, 1, 1, },
        {  0.03f, -0.60f, 0.0f, 0xff800000, 0, 1, }
	};

	CUSTOMVERTEX Vertices44[] =
    {
		{  1.23f, -0.60f, 0.0f, 0xffffffff, 1, 1, },
        {  0.63f, -0.00f, 0.0f, 0xff00ff00, 0, 0.5, },
        {  1.23f, -0.00f, 0.0f, 0xff800000, 1, 0.5, },

		{  0.63f, -0.00f, 0.0f, 0xff00ff00, 0, 0.5, },
        {  1.23f, -0.60f, 0.0f, 0xffffffff, 1, 1, },
        {  0.63f, -0.60f, 0.0f, 0xff800000, 0, 1, }
	};

    CUSTOMVERTEX Vertices5[] =
    {

		{ -1.23f, 0.60f, 0.0f, 0xffffffff, 0, 0, },
        { -0.63f, 0.00f, 0.0f, 0xff0000ff, 1, 0.5, },
        { -1.23f, 0.00f, 0.0f, 0xffff0000, 0, 0.5, },

		{ -0.63f, 0.00f, 0.0f, 0xff0000ff, 1, 0.5, },
        { -1.23f, 0.60f, 0.0f, 0xffffffff, 0, 0, },
        { -0.63f, 0.60f, 0.0f, 0xffff0000, 1, 0, }
	};

    CUSTOMVERTEX Vertices55[] =
    {

		{ -0.63f, 0.60f, 0.0f, 0xffffffff, 0, 0, },
        { -0.03f, 0.00f, 0.0f, 0xff0000ff, 1, 0.5, },
        { -0.63f, 0.00f, 0.0f, 0xffff0000, 0, 0.5, },

		{ -0.03f, 0.00f, 0.0f, 0xff0000ff, 1, 0.5, },
        { -0.63f, 0.60f, 0.0f, 0xffffffff, 0, 0, },
        { -0.03f, 0.60f, 0.0f, 0xffff0000, 1, 0, }
	};

	CUSTOMVERTEX Vertices6[] =
    {
		{  0.63f, 0.60f, 0.0f, 0xffffffff, 1, 0, },
        {  0.03f, 0.00f, 0.0f, 0xff0000ff, 0, 0.5, },
        {  0.63f, 0.00f, 0.0f, 0xffff0000, 1, 0.5, },

		{  0.03f, 0.00f, 0.0f, 0xff0000ff, 0, 0.5, },
        {  0.63f, 0.60f, 0.0f, 0xffffffff, 1, 0, },
        {  0.03f, 0.60f, 0.0f, 0xffff0000, 0, 0, }
	};

	CUSTOMVERTEX Vertices66[] =
    {
		{  1.23f, 0.60f, 0.0f, 0xffffffff, 1, 0, },
        {  0.63f, 0.00f, 0.0f, 0xff0000ff, 0, 0.5, },
        {  1.23f, 0.00f, 0.0f, 0xffff0000, 1, 0.5, },

		{  0.63f, 0.00f, 0.0f, 0xff0000ff, 0, 0.5, },
        {  1.23f, 0.60f, 0.0f, 0xffffffff, 1, 0, },
        {  0.63f, 0.60f, 0.0f, 0xffff0000, 0, 0, }
	};

	// Create the vertex buffer. Here we are allocating enough memory
    // (from the default pool) to hold all our 3 custom Vertices. We also
    // specify the FVF, so the vertex buffer knows what data it contains.
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB1, NULL ) ) )
    {
        return E_FAIL;
    }
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB2, NULL ) ) )
    {
        return E_FAIL;
    }
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB3, NULL ) ) )
    {
        return E_FAIL;
    }

	if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB4, NULL ) ) )
    {
        return E_FAIL;
    }

    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB5, NULL ) ) )
    {
        return E_FAIL;
    }

    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB6, NULL ) ) )
    {
        return E_FAIL;
    }

    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB11, NULL ) ) )
    {
        return E_FAIL;
    }
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB22, NULL ) ) )
    {
        return E_FAIL;
    }
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB33, NULL ) ) )
    {
        return E_FAIL;
    }

	if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB44, NULL ) ) )
    {
        return E_FAIL;
    }

    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB55, NULL ) ) )
    {
        return E_FAIL;
    }

    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB66, NULL ) ) )
    {
        return E_FAIL;
    }

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
    // gain access to the Vertices. This mechanism is required becuase vertex
    // buffers may be in device memory.
    VOID* pVertices;
    if( FAILED( g_pVB1->Lock( 0, sizeof( Vertices1 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices1, sizeof( Vertices1 ) );
    g_pVB1->Unlock();
    
	if( FAILED( g_pVB2->Lock( 0, sizeof( Vertices2 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices2, sizeof( Vertices2 ) );
    g_pVB2->Unlock();

	if( FAILED( g_pVB3->Lock( 0, sizeof( Vertices3 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices3, sizeof( Vertices3 ) );
    g_pVB3->Unlock();

	if( FAILED( g_pVB4->Lock( 0, sizeof( Vertices4 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices4, sizeof( Vertices4 ) );
    g_pVB4->Unlock();


	if( FAILED( g_pVB5->Lock( 0, sizeof( Vertices5 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices5, sizeof( Vertices5 ) );
    g_pVB5->Unlock();

	if( FAILED( g_pVB6->Lock( 0, sizeof( Vertices6 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices6, sizeof( Vertices6 ) );
    g_pVB6->Unlock();

	if( FAILED( g_pVB11->Lock( 0, sizeof( Vertices11 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices11, sizeof( Vertices11 ) );
    g_pVB11->Unlock();
    
	if( FAILED( g_pVB22->Lock( 0, sizeof( Vertices22 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices22, sizeof( Vertices22 ) );
    g_pVB22->Unlock();

	if( FAILED( g_pVB33->Lock( 0, sizeof( Vertices33 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices33, sizeof( Vertices33 ) );
    g_pVB33->Unlock();

	if( FAILED( g_pVB44->Lock( 0, sizeof( Vertices44 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices44, sizeof( Vertices44 ) );
    g_pVB44->Unlock();


	if( FAILED( g_pVB55->Lock( 0, sizeof( Vertices55 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices55, sizeof( Vertices55 ) );
    g_pVB55->Unlock();

	if( FAILED( g_pVB66->Lock( 0, sizeof( Vertices66 ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, Vertices66, sizeof( Vertices66 ) );
    g_pVB66->Unlock();

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pTexturesRight[1] != NULL )
        g_pTexturesRight[1]->Release();
    if( g_pTexturesRight[2] != NULL )
        g_pTexturesRight[2]->Release();
    if( g_pTexturesRight[3] != NULL )
        g_pTexturesRight[3]->Release();
    if( g_pTexturesRight[4] != NULL )
        g_pTexturesRight[4]->Release();
    if( g_pTexturesRight[5] != NULL )
        g_pTexturesRight[5]->Release();
    if( g_pTexturesRight[6] != NULL )
        g_pTexturesRight[6]->Release();
    if( g_pTexturesRight[7] != NULL )
        g_pTexturesRight[7]->Release();
    if( g_pTexturesRight[8] != NULL )
        g_pTexturesRight[8]->Release();
    if( g_pTexturesRight[9] != NULL )
        g_pTexturesRight[9]->Release();
    if( g_pTexturesRight[0] != NULL )
        g_pTexturesRight[0]->Release();
    if( g_pTexturesRight[11] != NULL )
        g_pTexturesRight[11]->Release();
    if( g_pTexturesRight[12] != NULL )
        g_pTexturesRight[12]->Release();
    if( g_pTexturesRight[13] != NULL )
        g_pTexturesRight[13]->Release();
    if( g_pTexturesRight[14] != NULL )
        g_pTexturesRight[14]->Release();
    if( g_pTexturesRight[15] != NULL )
        g_pTexturesRight[15]->Release();
    if( g_pTexturesRight[16] != NULL )
        g_pTexturesRight[16]->Release();
    if( g_pTexturesRight[17] != NULL )
        g_pTexturesRight[17]->Release();
    if( g_pTexturesRight[18] != NULL )
        g_pTexturesRight[18]->Release();
    if( g_pTexturesRight[19] != NULL )
        g_pTexturesRight[19]->Release();
    if( g_pTexturesRight[10] != NULL )
        g_pTexturesRight[10]->Release();
    if( g_pTexturesLeft[0] != NULL )
        g_pTexturesLeft[0]->Release();
    if( g_pTexturesLeft[1] != NULL )
        g_pTexturesLeft[1]->Release();
    if( g_pTexturesLeft[2] != NULL )
        g_pTexturesLeft[2]->Release();
    if( g_pTexturesLeft[3] != NULL )
        g_pTexturesLeft[3]->Release();
    if( g_pTexturesLeft[4] != NULL )
        g_pTexturesLeft[4]->Release();
    if( g_pTexturesLeft[5] != NULL )
        g_pTexturesLeft[5]->Release();
    if( g_pTexturesLeft[6] != NULL )
        g_pTexturesLeft[6]->Release();
    if( g_pTexturesLeft[7] != NULL )
        g_pTexturesLeft[7]->Release();
    if( g_pTexturesLeft[8] != NULL )
        g_pTexturesLeft[8]->Release();
    if( g_pTexturesLeft[9] != NULL )
        g_pTexturesLeft[9]->Release();
    if( g_pTexturesLeft[10] != NULL )
        g_pTexturesLeft[10]->Release();
    if( g_pTexturesLeft[11] != NULL )
        g_pTexturesLeft[11]->Release();

	if( g_pVB1 != NULL )
        g_pVB1->Release();
    if( g_pVB2 != NULL )
        g_pVB2->Release();
    if( g_pVB3 != NULL )
        g_pVB3->Release();
    if( g_pVB4 != NULL )
        g_pVB4->Release();
    if( g_pVB5 != NULL )
        g_pVB5->Release();
    if( g_pVB6 != NULL )
        g_pVB6->Release();
	if( g_pVB11 != NULL )
        g_pVB11->Release();
    if( g_pVB22 != NULL )
        g_pVB22->Release();
    if( g_pVB33 != NULL )
        g_pVB33->Release();
    if( g_pVB44 != NULL )
        g_pVB44->Release();
    if( g_pVB55 != NULL )
        g_pVB55->Release();
    if( g_pVB66 != NULL )
        g_pVB66->Release();


    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();

	if( pRenderToSurface != NULL )
        pRenderToSurface->Release();

	if( pDynamicTexture != NULL )
        pDynamicTexture->Release();

	if( pTextureSurface != NULL )
        pTextureSurface->Release();

	if( offScreenSurface != NULL)
		offScreenSurface->Release();

	ReleaseDC(hWnd, hDC);
}



VOID SetMyWorldTransform(FLOAT angle,FLOAT ox,FLOAT oy,FLOAT oz)
{
// For our world matrix, we will just rotate the object about the y-axis.
    D3DXMATRIXA16 matWorld, matOffset;

    // Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
    // every 1000 ms. To avoid the loss of precision inherent in very high 
    // floating point numbers, the system time is modulated by the rotation 
    // period before conversion to a radian angle.
	
	D3DXVECTOR3 axis(-1.0, 0.0f, 0.0f);
    D3DXMatrixRotationAxis(&matWorld, &axis, angle);
	D3DXMatrixTranslation(&matOffset, ox, oy, oz);
	matWorld=matOffset*matWorld;

    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld);
}

VOID SetupMatrices()
{
	// Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXVECTOR3 vEyePt( 0.0f, -0.61f, 3.0f );
    D3DXVECTOR3 vLookatPt( 0.f, -0.61f, -1.0f );
    D3DXVECTOR3 vUpVec( 0.0f, -1.0f, 0.0f );
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	// Begin the scene
    if( SUCCEEDED( pRenderToSurface->BeginScene( pTextureSurface, NULL ) ) )
    {
	    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
        // Draw the triangles in the vertex buffer. This is broken into a few
        // steps. We are passing the Vertices down a "stream", so first we need
        // to specify the source of that stream, which is our vertex buffer. Then
        // we need to let D3D know what vertex shader to use. Full, custom vertex
        // shaders are an advanced topic, but in most cases the vertex shader is
        // just the FVF, so that D3D knows what type of Vertices we are dealing
        // with. Finally, we call DrawPrimitive() which does the actual rendering
        // of our geometry (in this case, just one triangle).
 
		// Setup the world, view, and projection Matrices
        SetupMatrices();

		GetLocalTime(&st);

		FLOAT fAngle = 0;
		fAngle = ( st.wMilliseconds % 1000 ) * ( 1.0f * D3DX_PI ) / 1000.0f;
		SetMyWorldTransform(0,0,0,0);

		if ( st.wSecond == 59 )
		{
			if (st.wMinute!=59)
			{
				if ((st.wMinute-mm)%3==0) 
				{
					fAngleV3=-fAngle;
					if (st.wMilliseconds <= 500)
					{
						imin1 = st.wMinute / 10;
						jmin1 = st.wMinute % 10;
						imin2 = st.wMinute / 10;
						jmin2 = st.wMinute % 10;
						imin3 = (st.wMinute + 1) / 10;
						jmin3 = (st.wMinute + 1) % 10;
					}
					if (st.wMilliseconds > 500)
					{
						imin1 = (st.wMinute + 1) / 10 + 6;
						jmin1 = (st.wMinute + 1) % 10 + 10;
						imin2 = st.wMinute / 10;
						jmin2 = st.wMinute % 10;
						imin3 = (st.wMinute + 1) / 10;
						jmin3 = (st.wMinute + 1) % 10;
					}
				}

				if ((st.wMinute-mm)%3==1)
				{
					fAngleV6=-fAngle;
					if (st.wMilliseconds <= 500)
					{
						imin1 = st.wMinute / 10 + 6;
						jmin1 = st.wMinute % 10 + 10;
						imin2 = (st.wMinute + 1) / 10 + 6;
						jmin2 = (st.wMinute + 1) % 10 + 10;
						imin3 = st.wMinute / 10;
						jmin3 = st.wMinute % 10;
					}
					if (st.wMilliseconds > 500)
					{
						imin1 = st.wMinute / 10 + 6;
						jmin1 = st.wMinute % 10 + 10;
						imin2 = (st.wMinute + 1) / 10 + 6;
						jmin2 = (st.wMinute + 1) % 10 + 10;
						imin3 = (st.wMinute + 1) / 10 + 6;
						jmin3 = (st.wMinute + 1) % 10 + 10;
					}
				}
				if ((st.wMinute-mm)%3==2)
				{
					fAngleV4=-fAngle-D3DX_PI;
					if (st.wMilliseconds <= 500)
					{
						imin1 = (st.wMinute + 1) / 10;
						jmin1 = (st.wMinute + 1) % 10;
						imin2 = st.wMinute / 10 + 6;
						jmin2 = st.wMinute % 10 + 10;
						imin3 = st.wMinute / 10 + 6;
						jmin3 = st.wMinute % 10 + 10;
					}
					if (st.wMilliseconds > 500)
					{
						imin1 = (st.wMinute + 1) / 10;
						jmin1 = (st.wMinute + 1) % 10;
						imin2 = (st.wMinute + 1) / 10;
						jmin2 = (st.wMinute + 1) % 10;
						imin3 = st.wMinute / 10 + 6;
						jmin3 = st.wMinute % 10 + 10;
					}
				}
			}
			else
			{
				if ((st.wMinute-mm)%3==0)
				{
					fAngleV3=-fAngle;
					if (st.wMilliseconds <= 500)
					{
						imin1 = 5;
						jmin1 = 9;
						imin2 = 5;
						jmin2 = 9;
						imin3 = 0;
						jmin3 = 0;
					}
					if (st.wMilliseconds > 500)
					{
						imin1 = 6;
						jmin1 = 10;
						imin2 = 5;
						jmin2 = 9;
						imin3 = 0;
						jmin3 = 0;
					}	
				}
				if ((st.wMinute-mm)%3==1)
				{
					fAngleV6=-fAngle;
					if (st.wMilliseconds <= 500)
					{
						imin1 = 11;
						jmin1 = 19;
						imin2 = 6;
						jmin2 = 10;
						imin3 = 5;
						jmin3 = 9;
					}
					if (st.wMilliseconds > 500)
					{
						imin1 = 11;
						jmin1 = 19;
						imin2 = 6;
						jmin2 = 10;
						imin3 = 6;
						jmin3 = 10;
					}
				}
				if ((st.wMinute-mm)%3==2)
				{
					fAngleV4=-fAngle-D3DX_PI;
					if (st.wMilliseconds <= 500)
					{
						imin1 = 0;
						jmin1 = 0;
						imin2 = 11;
						jmin2 = 19;
						imin3 = 11;
						jmin3 = 19;
					}
					if (st.wMilliseconds > 500)
					{
						imin1 = 0;
						jmin1 = 0;
						imin2 = 0;
						jmin2 = 0;
						imin3 = 11;
						jmin3 = 19;
					}
				}
				if ((st.wHour-hh)%3==0)
				{
					fAngleV1=-fAngle; 
					if (st.wMilliseconds <= 500)
					{
						ihours1 = (st.wHour % 24) / 10;
						jhours1 = (st.wHour % 24) % 10;
						ihours2 = (st.wHour % 24) / 10;
						jhours2 = (st.wHour % 24) % 10;
						ihours3 = ((st.wHour + 1) % 24) / 10;
						jhours3 = ((st.wHour + 1) % 24) % 10;
					}
					if (st.wMilliseconds > 500)
					{
						ihours1 = ((st.wHour + 1) % 24) / 10 + 6;
						jhours1 = ((st.wHour + 1) % 24) % 10 + 10;
						ihours2 = (st.wHour % 24) / 10;
						jhours2 = (st.wHour % 24) % 10;
						ihours3 = ((st.wHour + 1) % 24) / 10;
						jhours3 = ((st.wHour + 1) % 24) % 10;
					}
				}
				if ((st.wHour-hh)%3==1)
				{
					fAngleV5=-fAngle; 
					if (st.wMilliseconds <= 500)
					{
						ihours1 = (st.wHour % 24) / 10 + 6;
						jhours1 = (st.wHour % 24) % 10 + 10;
						ihours2 = (st.wHour % 24) / 10 + 6;
						jhours2 = (st.wHour % 24) % 10 + 10;
						ihours3 = (st.wHour % 24) / 10;
						jhours3 = (st.wHour % 24) % 10;
					}
					if (st.wMilliseconds > 500)
					{
						ihours1 = (st.wHour % 24) / 10 + 6;
						jhours1 = (st.wHour % 24) % 10 + 10;
						ihours2 = (st.wHour % 24) / 10 + 6;
						jhours2 = (st.wHour % 24) % 10 + 10;
						ihours3 = (st.wHour % 24) / 10 + 6;
						jhours3 = (st.wHour % 24) % 10 + 10;
					}
				}
				if ((st.wHour-hh)%3==2)
				{
					fAngleV2=-fAngle-D3DX_PI; 
					if (st.wMilliseconds <= 500)
					{
						ihours1 = ((st.wHour + 4) % 24) / 10;
						jhours1 = ((st.wHour + 4) % 24) % 10;
						ihours2 = (st.wHour % 24) / 10 + 6;
						jhours2 = (st.wHour % 24) % 10 + 10;
						ihours3 = (st.wHour % 24) / 10 + 6;
						jhours3 = (st.wHour % 24) % 10 + 10;
					}
					if (st.wMilliseconds > 500)
					{
						ihours1 = ((st.wHour + 1) % 24) / 10;
						jhours1 = ((st.wHour + 1) % 24) % 10;
						ihours2 = ((st.wHour + 1) % 24) / 10;
						jhours2 = ((st.wHour + 1) % 24) % 10;
						ihours3 = (st.wHour % 24) / 10 + 6;
						jhours3 = (st.wHour % 24) % 10 + 10;
					}
				}
			}
		}

		if ( st.wSecond == 0 )
		{
			if ((st.wHour-hh)%3==0)
			{
				fAngleV1=0;
				fAngleV5=0;
				fAngleV2=0;
				ihours1 = (st.wHour % 24) / 10;
				jhours1 = (st.wHour % 24) % 10;
				ihours2 = (st.wHour % 24) / 10;
				jhours2 = (st.wHour % 24) % 10;
				ihours3 = (st.wHour % 24) / 10;
				jhours3 = (st.wHour % 24) % 10;
			}

			if ((st.wHour-hh)%3==1)
			{
				fAngleV1=D3DX_PI;
				fAngleV5=0;
				fAngleV2=D3DX_PI;
				ihours1 = (st.wHour % 24) / 10 + 6;
				jhours1 = (st.wHour % 24) % 10 + 10;
				ihours2 = (st.wHour % 24) / 10;
				jhours2 = (st.wHour % 24) % 10;
				ihours3 = (st.wHour % 24) / 10;
				jhours3 = (st.wHour % 24) % 10;
			}

			if ((st.wHour-hh)%3==2)
			{
				fAngleV1=0;
				fAngleV5=D3DX_PI;
				fAngleV2=D3DX_PI;
				ihours1 = (st.wHour % 24) / 10;
				jhours1 = (st.wHour % 24) % 10;
				ihours2 = (st.wHour % 24) / 10 + 6;
				jhours2 = (st.wHour % 24) % 10 + 10;
				ihours3 = (st.wHour % 24) / 10 + 6;
				jhours3 = (st.wHour % 24) % 10 + 10;
			}
		
			if ((st.wMinute-mm)%3==0)
			{
				fAngleV3=0;
				fAngleV6=0;
				fAngleV4=0;
				imin1 = st.wMinute / 10;
				jmin1 = st.wMinute % 10;
				imin2 = st.wMinute / 10;
				jmin2 = st.wMinute % 10;
				imin3 = st.wMinute / 10;
				jmin3 = st.wMinute % 10;
			}

			if ((st.wMinute-mm)%3==1)
			{
				fAngleV3=D3DX_PI;
				fAngleV6=0;
				fAngleV4=D3DX_PI;
				imin1 = st.wMinute / 10 + 6;
				jmin1 = st.wMinute % 10 + 10;
				imin2 = st.wMinute / 10;
				jmin2 = st.wMinute % 10;
				imin3 = st.wMinute / 10;
				jmin3 = st.wMinute % 10;
			}

			if ((st.wMinute-mm)%3==2)
			{
				fAngleV3=0;
				fAngleV6=D3DX_PI;
				fAngleV4=D3DX_PI;
				imin1 = st.wMinute / 10;
				jmin1 = st.wMinute % 10;
				imin2 = st.wMinute / 10 + 6;
				jmin2 = st.wMinute % 10 + 10;
				imin3 = st.wMinute / 10 + 6;
				jmin3 = st.wMinute % 10 + 10;
			}
		}

		SetMyWorldTransform(fAngleV3,0,0,0);
		g_pd3dDevice->SetStreamSource( 0, g_pVB3, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesLeft[imin1] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
		g_pd3dDevice->SetStreamSource( 0, g_pVB33, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesRight[jmin1] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

		SetMyWorldTransform(fAngleV2,0,0,0);
		g_pd3dDevice->SetStreamSource( 0, g_pVB2, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesLeft[ihours2] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
		g_pd3dDevice->SetStreamSource( 0, g_pVB22, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesRight[jhours2] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

		SetMyWorldTransform(fAngleV1,0,0,0);
		g_pd3dDevice->SetStreamSource( 0, g_pVB1, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesLeft[ihours1] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
		g_pd3dDevice->SetStreamSource( 0, g_pVB11, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesRight[jhours1] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

		SetMyWorldTransform(fAngleV4,0,0,0);
		g_pd3dDevice->SetStreamSource( 0, g_pVB4, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesLeft[imin2] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
		g_pd3dDevice->SetStreamSource( 0, g_pVB44, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesRight[jmin2] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

		SetMyWorldTransform(fAngleV5,0,0,0);
		g_pd3dDevice->SetStreamSource( 0, g_pVB5, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesLeft[ihours3] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
		g_pd3dDevice->SetStreamSource( 0, g_pVB55, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesRight[jhours3] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

		SetMyWorldTransform(fAngleV6,0,0,0);
		g_pd3dDevice->SetStreamSource( 0, g_pVB6, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesLeft[imin3] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
		g_pd3dDevice->SetStreamSource( 0, g_pVB66, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->SetTexture( 0, g_pTexturesRight[jmin3] );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

		// End the scene
		pRenderToSurface->EndScene(0);
		
		fAngle = 0;
    }

    // Present the backbuffer contents to the display
//    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	D3DLOCKED_RECT Data;
	RECT Rect = { 0, 0, WND_W, WND_H };

	g_pd3dDevice->GetRenderTargetData(pTextureSurface, offScreenSurface);
	HRESULT HR = offScreenSurface->LockRect( &Data, &Rect, D3DLOCK_READONLY);
	SetDIBitsToDevice( hDC, 0, 0, WND_W, WND_W, 0, 0, 0, WND_H, Data.pBits, &BmpInfo, DIB_RGB_COLORS);
	offScreenSurface->UnlockRect();

}

bool __fastcall CreateShortCut( 
							   LPWSTR pwzShortCutFileName, 
							   LPTSTR pszPathAndFileName, 
							   LPTSTR pszWorkingDirectory, 
							   LPTSTR pszArguments, 
							   WORD wHotKey, 
							   int iCmdShow, 
							   LPTSTR pszIconFileName, 
							   int iIconIndex) 
{ 
					   IShellLink * pSL; 
	IPersistFile * pPF; 
	HRESULT hRes; 
	if( CoInitialize(NULL) != S_OK)
		return false;
	// Получение экземпляра компонента "Ярлык" 
	hRes = CoCreateInstance(CLSID_ShellLink, 0,	CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&pSL); 

	if( SUCCEEDED(hRes) ) 
	{ 
		hRes = pSL->SetPath(pszPathAndFileName); 
		if( SUCCEEDED(hRes) ) 
		{ 
			//hRes = pSL->SetArguments(pszArguments); 
			//if( SUCCEEDED(hRes) ) 
			{ 
				hRes = pSL->SetWorkingDirectory(pszWorkingDirectory); 
				if( SUCCEEDED(hRes) ) 
				{ 
					hRes = pSL->SetIconLocation(pszIconFileName,iIconIndex); 
					if( SUCCEEDED(hRes) ) 
					{ 
					//	hRes = pSL->SetHotkey(wHotKey); 
					//	if( SUCCEEDED(hRes) ) 
						{ 
							hRes = pSL->SetShowCmd(iCmdShow); 
							if( SUCCEEDED(hRes) ) 
							{ 
								// Получение компонента хранилища параметров 
								hRes = pSL->QueryInterface(IID_IPersistFile,(LPVOID *)&pPF); 
								if( SUCCEEDED(hRes) ) 
								{ 
									// Сохранение созданного ярлыка 
									hRes = pPF->Save(pwzShortCutFileName,TRUE); 
									pPF->Release(); 
								} 
							} 
						} 
					} 
				} 
			} 
		} 
		pSL->Release(); 
	} 
	return SUCCEEDED(hRes); 

}  

VOID AutorunADD()
{
    TCHAR szPath[0x100]; 
    GetModuleFileName(NULL, szPath, sizeof(szPath));
	LPITEMIDLIST pidl; 
	TCHAR buffer[256];
	SHGetSpecialFolderLocation( hWnd, CSIDL_ALTSTARTUP, &pidl );
	if (pidl==NULL)
	{
		MessageBox::Show( L"Error", L"", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}
	SHGetPathFromIDList( pidl, buffer );
    TCHAR szPath2[0x100]; 
	GetCurrentDirectory( sizeof(szPath2), szPath2 );

	CreateShortCut(wcscat( buffer, L"\\RetroClock.lnk" ), szPath, szPath2, L"", 0, SW_SHOWNORMAL, szPath, 0 );  
}

VOID AutorunDEL()
{
	LPITEMIDLIST pidl; 
	TCHAR buffer[256];
	SHGetSpecialFolderLocation( hWnd, CSIDL_ALTSTARTUP, &pidl );
	if (pidl==NULL)
	{
		MessageBox::Show( L"Error", L"", MessageBoxButtons::OK, MessageBoxIcon::Error );
	}
	SHGetPathFromIDList( pidl, buffer );
	DeleteFile( wcscat( buffer, L"\\RetroClock.lnk" ) );
}

VOID Checking()
{
	for (int i=0;i<5;i++)
		if (RButton == i) R[i] = true; else R[i] = false;
}

VOID ButtonR()
{
	switch (RButton)
	{
	case 0:
		{
			WND_W = 100;
			WND_H = 100;
		}break;
	case 1:
		{
			WND_W = 150;
			WND_H = 150;
		}break;
	case 2:
		{
			WND_W = 200;
			WND_H = 200;
		}break;
	case 3:
		{
			WND_W = 250;
			WND_H = 250;
		}break;
	case 4:
		{
			WND_W = 300;
			WND_H = 300;
		}break;
	}
}

VOID SaveSettings()
{
	HKEY hKey; 
    TCHAR szPath[0x100]; 
    GetModuleFileName(NULL, szPath, sizeof(szPath));
    RegCreateKeyEx(HKEY_CURRENT_USER, 
                     L"Software\\RetroClock\\", 
                     NULL, 
                     L"", 
                     REG_OPTION_NON_VOLATILE, 
                     KEY_WRITE | KEY_CREATE_SUB_KEY, 
                     NULL, 
                     &hKey, 
                     NULL); 

	POINT p;
	WindowFromPoint( p ); 
	wchar_t wsChx[0x100];
	LPCWSTR lpTextx;
	int iNumberx = Posx;
	 _itow_s( iNumberx, wsChx, 10 );
	lpTextx =wsChx;
	wchar_t wsChy[0x100];
	LPCWSTR lpTexty;
	int iNumbery = Posy;
	 _itow_s( iNumbery, wsChy, 10 );
	lpTexty =wsChy;

	wchar_t wsR[0x100];
	LPCWSTR lpTextR;
	int iNumberR = RButton;
	 _itow_s( iNumberR, wsR, 10 );
	lpTextR =wsR;
/*
	wchar_t wsW[0x100];
	LPCWSTR lpTextW;
	int iNumberW = WND_W;
	 _itow_s( iNumberW, wsW, 10 );
	lpTextR =wsW;

	wchar_t wsH[0x100];
	LPCWSTR lpTextH;
	int iNumberH = WND_H;
	 _itow_s( iNumberH, wsH, 10 );
	lpTextR =wsH;
*/
	int check = 0;
	if (AutorunChecked) check = 1;
	wchar_t wsChCh[0x100];
	LPCWSTR lpTextCh;
	 _itow_s( check, wsChCh, 10 );
	lpTextCh = wsChCh;

	int checkT = 0;
	if (TrayChecked) checkT = 1;
	wchar_t wsChChT[0x100];
	LPCWSTR lpTextChT;
	 _itow_s( checkT, wsChChT, 10 );
	lpTextChT = wsChChT;

	int checkTb = 0;
	if (TaskbarChecked) checkTb = 1;
	wchar_t wsChChTb[0x100];
	LPCWSTR lpTextChTb;
	 _itow_s( checkTb, wsChChTb, 10 );
	lpTextChTb = wsChChTb;

	wchar_t wsT[0x100];
	LPCWSTR lpTextT;
	 _itow_s( WindowVisIndex, wsT, 10 );
	lpTextT = wsT;

	if (hKey) 
    { 
        RegSetValueEx(hKey, L"X", NULL, REG_SZ, (LPBYTE)wsChx, (_tcslen(wsChx))*sizeof(_TCHAR) ); 
        RegSetValueEx(hKey, L"Y", NULL, REG_SZ, (LPBYTE)wsChy, (_tcslen(wsChy))*sizeof(_TCHAR) ); 
        RegSetValueEx(hKey, L"Size", NULL, REG_SZ, (LPBYTE)wsR, (_tcslen(wsR))*sizeof(_TCHAR) ); 
        RegSetValueEx(hKey, L"Transparance", NULL, REG_SZ, (LPBYTE)wsT, (_tcslen(wsT))*sizeof(_TCHAR) ); 
//		RegSetValueEx(hKey, L"Width", NULL, REG_SZ, (LPBYTE)wsW, (_tcslen(wsW))*sizeof(_TCHAR) ); 
//		RegSetValueEx(hKey, L"Height", NULL, REG_SZ, (LPBYTE)wsH, (_tcslen(wsH))*sizeof(_TCHAR) ); 
        RegSetValueEx(hKey, L"AutorunChecked", NULL, REG_SZ, (LPBYTE)wsChCh, (_tcslen(wsChCh))*sizeof(_TCHAR) ); 
        RegSetValueEx(hKey, L"TrayChecked", NULL, REG_SZ, (LPBYTE)wsChChT, (_tcslen(wsChChT))*sizeof(_TCHAR) ); 
        RegSetValueEx(hKey, L"TaskbarChecked", NULL, REG_SZ, (LPBYTE)wsChChTb, (_tcslen(wsChChTb))*sizeof(_TCHAR) ); 
        RegSetValueEx(hKey, L"Textures", NULL, REG_SZ, (LPBYTE)szPathTextures, (_tcslen(szPathTextures))*sizeof(_TCHAR) ); 
        RegCloseKey(hKey);
    }
}


VOID LoadSettings()
{
	int Checked;
	int CheckedT;
	int CheckedTb;
	HKEY phKey;
	TCHAR lpValueR[MAX_PATH];
//	TCHAR lpValueW[MAX_PATH];
//	TCHAR lpValueH[MAX_PATH];
	TCHAR lpValueX[MAX_PATH];
	TCHAR lpValueY[MAX_PATH];
	TCHAR lpValueT[MAX_PATH];
	TCHAR lpValueCh[MAX_PATH];
	TCHAR lpValueChT[MAX_PATH];
	TCHAR lpValueChTb[MAX_PATH];
	DWORD dwBufLenX = MAX_PATH;
	DWORD dwBufLenY = MAX_PATH;
	DWORD dwBufLenT = MAX_PATH;
	DWORD dwBufLenR = MAX_PATH;
//	DWORD dwBufLenW = MAX_PATH;
//	DWORD dwBufLenH = MAX_PATH;
	DWORD dwBufLenCh = MAX_PATH;
	DWORD dwBufLenChT = MAX_PATH;
	DWORD dwBufLenChTb = MAX_PATH;
	DWORD dwBufLenTx = MAX_PATH;

	if (RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\RetroClock\\", 0, KEY_ALL_ACCESS, &phKey )==ERROR_SUCCESS)
	{
		if ((RegQueryValueEx( phKey, L"X", NULL, NULL, (LPBYTE)lpValueX, &dwBufLenX )== ERROR_SUCCESS) & (RegQueryValueEx( phKey, L"Y", NULL, NULL, (LPBYTE)lpValueY, &dwBufLenY )==ERROR_SUCCESS))
		{
			Posx = _wtoi( lpValueX );
			Posy = _wtoi( lpValueY );
		}
		
		if (RegQueryValueEx( phKey, L"Size", NULL, NULL, (LPBYTE)lpValueR, &dwBufLenR )== ERROR_SUCCESS)
		{
			RButton = _wtoi( lpValueR );
			ButtonR();
		}

		if (RegQueryValueEx( phKey, L"Transparance", NULL, NULL, (LPBYTE)lpValueT, &dwBufLenT )== ERROR_SUCCESS)
		{
			WindowVisIndex = _wtoi( lpValueT );
		}
/*
		if ((RegQueryValueEx( phKey, L"Width", NULL, NULL, (LPBYTE)lpValueW, &dwBufLenW )==ERROR_SUCCESS) & (RegQueryValueEx( phKey, L"Height", NULL, NULL, (LPBYTE)lpValueH, &dwBufLenH )==ERROR_SUCCESS))
		{
			WND_W = _wtoi( lpValueW );
			WND_H = _wtoi( lpValueH );
		}
*/		
		if (RegQueryValueEx( phKey, L"AutorunChecked", NULL, NULL, (LPBYTE)lpValueCh, &dwBufLenCh )== ERROR_SUCCESS)
		{
			Checked = _wtoi( lpValueCh );
			if ( Checked == 1 )	AutorunChecked = TRUE; else	AutorunChecked = FALSE;
		}

		if (RegQueryValueEx( phKey, L"TrayChecked", NULL, NULL, (LPBYTE)lpValueChT, &dwBufLenChT )== ERROR_SUCCESS)
		{
			CheckedT = _wtoi( lpValueChT );
			if ( CheckedT == 1 ) TrayChecked = TRUE; else TrayChecked = FALSE;
		}

		if (RegQueryValueEx( phKey, L"TaskbarChecked", NULL, NULL, (LPBYTE)lpValueChTb, &dwBufLenChTb )== ERROR_SUCCESS)
		{
			CheckedTb = _wtoi( lpValueChTb );
			if ( CheckedTb == 1 ) TaskbarChecked = TRUE; else TaskbarChecked = FALSE;
		}
	}

	RegQueryValueEx( phKey, L"Textures", NULL, NULL, (LPBYTE)szPathTextures, &dwBufLenTx );

	Checking();

	RegCloseKey( phKey );
}

VOID AddToTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof( nid );
	nid.hWnd = hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.hIcon = LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_ICON1 ) );
	lstrcpyn(nid.szTip, _T("RetroClock"), sizeof(nid.szTip)/sizeof(nid.szTip[0]));
	nid.uCallbackMessage = WM_SHELLNOTE;

	Shell_NotifyIcon( NIM_ADD, &nid );
}

VOID DelFromTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof( nid );
	nid.hWnd = hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.hIcon = LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_ICON1 ) );
	lstrcpyn(nid.szTip, _T("RetroClock"), sizeof(nid.szTip)/sizeof(nid.szTip[0]));
	nid.uCallbackMessage = WM_SHELLNOTE;

	Shell_NotifyIcon( NIM_DELETE, &nid );
}

VOID HideIconT()
{
	ShowWindow( hWnd, SW_HIDE );
	LONG wstyle;
	wstyle = GetWindowLong( hWnd, GWL_EXSTYLE );			
	wstyle|=WS_EX_TOOLWINDOW;
	SetWindowLong( hWnd, GWL_EXSTYLE, wstyle );
	ShowWindow ( hWnd, SW_SHOW );
}

VOID ShowIconT()
{
	ShowWindow( hWnd, SW_HIDE );
	LONG wstyle;
	wstyle = GetWindowLong( hWnd, GWL_EXSTYLE );			
	wstyle = WS_EX_LAYERED;
	SetWindowLong( hWnd, GWL_EXSTYLE, wstyle );
	ShowWindow ( hWnd, SW_SHOW );
}

VOID TranslaterS(String ^s) {
	CString cs(s);
	wchar_t* buf = cs.GetBuffer(0);
	_tcscpy( szPathTextures, buf );
}

void Reset() 
{
	TCHAR szPath[0x100];
	GetModuleFileName(NULL, szPath, sizeof(szPath));
	SaveSettings();
	Cleanup();
	STARTUPINFO start={0};
	start.cb = sizeof( start );
	PROCESS_INFORMATION pinf={0};
	CreateProcess( NULL, szPath, NULL, NULL, NULL, NULL, NULL, NULL, &start, &pinf );
	ExitProcess( 0x0 );
}


using namespace Vertices;

[STAThreadAttribute]

void main()
{
// Включение визуальных эффектов Windows XP до создания каких-либо элементов управления
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

// Создание главного окна и его запуск
	String ^s = gcnew String((LPCTSTR)szPathTextures);
	Settings ^set = Settings::GetSet();
	set->radioButton1->Checked = R[0];
	set->radioButton2->Checked = R[1];
	set->radioButton3->Checked = R[2];
	set->radioButton4->Checked = R[3];
	set->radioButton5->Checked = R[4];
	set->textBox1->Text = s;
	set->checkBox1->Checked = TaskbarChecked;
	set->checkBox2->Checked = TrayChecked;
	set->checkBox3->Checked = AutorunChecked;
	set->hScrollBar1->Value = WindowVisIndex;
	set->label5->Text = WindowVisIndex.ToString() + "%";
	Application::Run(set);
}	

void visibility() 
{
	String ^s = gcnew String((LPCTSTR)szPathTextures);
	Settings ^set = Settings::GetSet();
	set->radioButton1->Checked = R[0];
	set->radioButton2->Checked = R[1];
	set->radioButton3->Checked = R[2];
	set->radioButton4->Checked = R[3];
	set->radioButton5->Checked = R[4];
	set->textBox1->Text = s;
	set->checkBox1->Checked = TaskbarChecked;
	set->checkBox2->Checked = TrayChecked;
	set->checkBox3->Checked = AutorunChecked;
	set->button1->Enabled = false;
	set->Show();
}

bool Compare(String ^s) {
	String ^str = gcnew String((LPCTSTR)szPathTextures);
	bool c = String::Equals(s, str);
	return c;
}

void WindowVisibility(int index)
{
	SetLayeredWindowAttributes( hWnd, 0, index, LWA_ALPHA|LWA_COLORKEY );
	Render();
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
    {
/*	case WM_CREATE:
		{
			SetLayeredWindowAttributes(hWnd, 0, 100, LWA_COLORKEY|LWA_ALPHA);
		}
		break;
*/
	case WM_SHELLNOTE:
		{
			if ( (UINT)lParam == WM_RBUTTONDOWN )
			{
				SetForegroundWindow(hWnd);
				POINT cp;
				GetCursorPos(&cp);
				TrackPopupMenuEx( hMenu, TPM_HORIZONTAL | TPM_LEFTALIGN, cp.x, cp.y, hWnd, NULL);
			}
		}
	case WM_TIMER: 
		{
			if (Alpha < (WindowVisIndex*255/100))
			{
				Alpha+=1;
				SetLayeredWindowAttributes( hWnd, 0, Alpha, LWA_ALPHA|LWA_COLORKEY );
			}
			Render();
		}
		break;
	case WM_MOVE:
		{
		  Posx = LOWORD(lParam);
		  Posy = HIWORD(lParam);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_ADMINISTRATOR: ShellExecute( hWnd, L"open", L"Administrator.exe", NULL, NULL, SW_SHOWNORMAL );break; 
		case ID_EXIT: SendMessage( hWnd, WM_DESTROY, NULL, NULL); break;
		case ID_SETTINGS:
			{
				if (SetS) visibility(); else main();
			}break;
		}
		break;

	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE: SendMessage( hWnd, WM_DESTROY, NULL, NULL);break;
			}
		}
		break;

	case WM_RBUTTONDOWN:
		{
			POINT cp;
			GetCursorPos(&cp);
			TrackPopupMenuEx( hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, cp.x, cp.y, hWnd, NULL);
		}

	case WM_LBUTTONDOWN:
		{
			if (wParam == MK_LBUTTON) SendMessage( hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
		break;

	case WM_CLOSE:
		{
			PostQuitMessage( 0 );
		}
		break;

	case WM_DESTROY:
			if((MessageBox::Show(
         "Are you sure?", 
         "Exiting", MessageBoxButtons::YesNo, 
         MessageBoxIcon::Question) == DialogResult::Yes))
			{
				SaveSettings();
				DelFromTray();
				Cleanup();
				PostQuitMessage( 0 );
			}
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    UNREFERENCED_PARAMETER( hInst );

	// Register the window class
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC | CS_DBLCLKS, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON1 ) ), NULL, NULL, NULL,
        L"D3D", NULL
    };
    RegisterClassEx( &wc );

	LoadSettings();

	hWnd = CreateWindowEx( WS_EX_LAYERED , L"D3D", L"RetroClock",
                              WS_POPUP | WS_VISIBLE , Posx, Posy, WND_W, WND_H,
                              NULL, NULL, wc.hInstance, NULL );

	AppendMenu((hMenu = CreatePopupMenu()), MF_ENABLED, ID_ADMINISTRATOR, L"Administrator ;-)" );
	AppendMenu(hMenu, MF_ENABLED | MF_SEPARATOR, NULL, L"");
	if ( TrayChecked == TRUE ) AddToTray();
	if ( TaskbarChecked == FALSE ) HideIconT();

	AppendMenu(hMenu, MF_ENABLED, ID_SETTINGS, L"Settings");
	AppendMenu(hMenu, MF_ENABLED | MF_SEPARATOR, NULL, L"");
	AppendMenu(hMenu, MF_ENABLED, ID_EXIT, L"Exit");

	SetMenu( hWnd, hMenu );

	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_COLORKEY|LWA_ALPHA);

	BITMAP Bmp;

	hDC = GetDC(hWnd);
	::GetObject((HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP), sizeof(Bmp), (LPVOID)&Bmp);

	BITMAPINFOHEADER& BmpIH = BmpInfo.bmiHeader;
	BmpIH.biSize = sizeof(BITMAPINFOHEADER);
	BmpIH.biWidth = Bmp.bmWidth;
	BmpIH.biHeight = Bmp.bmHeight;
	BmpIH.biPlanes = Bmp.bmPlanes;
	BmpIH.biBitCount = Bmp.bmBitsPixel;
	BmpIH.biCompression = BI_RGB;
	BmpIH.biSizeImage = ((Bmp.bmWidth * Bmp.bmBitsPixel + 31)&(~31))/8 * Bmp.bmHeight;
	BmpIH.biXPelsPerMeter = 0;
	BmpIH.biYPelsPerMeter = 0;
	BmpIH.biClrUsed = 0;
	BmpIH.biClrImportant = 0;

	// Initialize Direct3D
    if( SUCCEEDED( InitD3D() ) )
    {
        // Create the vertex buffer
	   if( SUCCEEDED( InitGeometry() ) )
        {
            // Show the window
            UpdateWindow( hWnd );
	    	GetLocalTime(&st);  
			
			hh=st.wHour;
			mm=st.wMinute;
			imin1 = st.wMinute / 10;
			jmin1 = st.wMinute % 10;
			imin2 = st.wMinute / 10;
			jmin2 = st.wMinute % 10;
			imin3 = st.wMinute / 10;
			jmin3 = st.wMinute % 10;

			ihours1 = (st.wHour % 24) / 10;
			jhours1 = (st.wHour % 24) % 10;
			ihours2 = (st.wHour % 24) / 10;
			jhours2 = (st.wHour % 24) % 10;
			ihours3 = (st.wHour % 24) / 10;
			jhours3 = (st.wHour % 24) % 10;

			// Enter the message loop
            MSG msg;
            ZeroMemory( &msg, sizeof( msg ) );

			SetTimer( hWnd, 1, 4, NULL );
            
			while( msg.message != WM_QUIT )
            {
                if( GetMessage( &msg, NULL, 0, 0) )
                {
                    TranslateMessage( &msg );
					DispatchMessage( &msg ); 
                }
            }
			KillTimer( hWnd, 1 );
        }
    }

    UnregisterClass( L"D3D", wc.hInstance );
    return 0;
}