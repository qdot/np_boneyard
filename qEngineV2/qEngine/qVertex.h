/********************************************************************
	created:	2002/11/21
	created:	21:11:2002   19:44
	filename: 	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine\qvertex.h
	file path:	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine
	file base:	qvertex
	file ext:	h
	author:		qDot <qDot@numberporn.com>
	
	purpose:	Header file which contains all of the FVF specs for 
				the qEngine library.

				I stole parts of this class from J. Carmer's 
				bananaengine at http://www.undergroundrainforest.com/

				Honestly, it's not like I couldn't of written it 
				myself. But why write when you can cut and paste. Not
				to mention, this isn't exactly new stuff.
*********************************************************************/

#pragma once
#include <d3dx8.h>

// Simple FVF
struct POINT_VERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;
};
const DWORD D3DFVF_PointVertex = D3DFVF_XYZ | D3DFVF_DIFFUSE;

// The Vertex FVF
const DWORD D3DFVF_TexturedVertex = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
struct TEX_VERTEX
{
	D3DXVECTOR3 pos;
	DWORD color;
	float tu;
	float tv;
};

// The Processed Vertex FVF
const DWORD D3DFVF_TexturedProcessedVertex = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
struct TEXPROC_VERTEX
{
	D3DXVECTOR3 pos;
	float rhw;
	DWORD color;
	float tu;
	float tv;
};

const DWORD D3DFVF_LitVertex = D3DFVF_XYZ | D3DFVF_NORMAL;
struct LIT_VERTEX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
};

const DWORD D3DFVF_LitTexturedVertex = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
struct LITTEX_VERTEX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	DWORD color;
	float tu;
	float tv;
};