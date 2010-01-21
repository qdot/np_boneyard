#include "qPosition.h"

namespace qEngine
{

	qPosition::qPosition(void) : 
		m_fXPos(0), m_fYPos(0), m_fZPos(0),
		m_fXRotation(0), m_fYRotation(0), m_fZRotation(0),
		m_fXScale(0), m_fYScale(0), m_fZScale(0),
		m_bBillboard(false), m_matCombine1(NULL), m_matCombine2(NULL)
	{
		move(0.0f,0.0f,0.0f);
		rotate(0.0f,0.0f,0.0f);
		scale(1.0f, 1.0f, 1.0f);
		updateMatrix();
	}

	qPosition::~qPosition(void)
	{
	}

	inline void qPosition::position(float fXPos, float fYPos, float fZPos)
	{
		m_fXPos = fXPos;
		m_fYPos = fYPos;
		m_fZPos = fZPos;
	}

	inline void qPosition::positionRelative(float fXAdd, float fYAdd, float fZAdd)
	{
		position(m_fXPos + fXAdd, m_fYPos + fYAdd, m_fZPos + fZAdd);
	}

	void qPosition::move(float fXPos, float fYPos, float fZPos)
	{  
		position(fXPos, fYPos, fZPos);
		D3DXMatrixTranslation(&m_matTranslation, m_fXPos, m_fYPos, m_fZPos);
	}

	void qPosition::moveRelative(float fXAdd, float fYAdd, float fZAdd)
	{
		move(m_fXPos + fXAdd, m_fYPos + fYAdd, m_fZPos + fZAdd);
	}

	void qPosition::rotate(float fXRot, float fYRot, float fZRot)
	{
		m_fXRotation = fXRot;
		m_fYRotation = fYRot;
		m_fZRotation = fZRot;

		D3DXMatrixRotationYawPitchRoll(&m_matRotation, m_fYRotation, m_fXRotation, m_fZRotation);
	}

	void qPosition::rotateRelative(float fXAdd, float fYAdd, float fZAdd)
	{
		rotate(m_fXRotation + fXAdd, m_fYRotation + fYAdd, m_fZRotation + fZAdd);
	}

	void qPosition::scale(float fXScale, float fYScale, float fZScale)
	{
		m_fXScale = fXScale;
		m_fYScale = fYScale;
		m_fZScale = fZScale;

		D3DXMatrixScaling(&m_matScale, m_fXScale, m_fYScale, m_fZScale);
	}
	
	void qPosition::scaleRelative(float fXAdd, float fYAdd, float fZAdd)
	{
		return scale(m_fXScale + fXAdd, m_fYScale + fYAdd, m_fZScale + fZAdd);
	}

	void qPosition::updateMatrix()
	{
		D3DXMATRIX matView, matTransposed;
		
		/*
		// Setup billboarding matrix
		if(m_Billboard == TRUE) {
			if(Graphics != NULL && Graphics->GetDeviceCOM() != NULL) {
			Graphics->GetDeviceCOM()->GetTransform(D3DTS_VIEW, &matView);
			D3DXMatrixTranspose(&matTransposed, &matView);
			matTransposed._41 = matTransposed._42 = matTransposed._43 = matTransposed._14 = matTransposed._24 = matTransposed._34 = 0.0f;
			} else {
			D3DXMatrixIdentity(&matTransposed);
			}
		}
		*/

		// Combine scaling and rotation matrices first
		D3DXMatrixMultiply(&m_matWorld, &m_matScale, &m_matRotation);
		
		/*
		// Apply billboard matrix
		if(m_Billboard == TRUE)
			D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &matTransposed);  
		*/

		// Combine with translation matrix
		D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &m_matTranslation);

		// Combine with combined matrices (if any)
		if(m_matCombine1 != NULL) 
			D3DXMatrixMultiply(&m_matWorld, &m_matWorld, m_matCombine1);
		if(m_matCombine2 != NULL) 
			D3DXMatrixMultiply(&m_matWorld, &m_matWorld, m_matCombine2);
	}

	void qPosition::setView()
	{
		g_lpD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	}

}