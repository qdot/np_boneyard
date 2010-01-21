#pragma once

#include <qEngine/qCommonInclude.h>

namespace qEngine
{

	class qPosition
	{
	public:
		qPosition(void);
		~qPosition(void);

		void updateMatrix();
		void setView();

        inline void setCombineMatrix1(D3DXMATRIX *matIn = NULL) { m_matCombine1 = matIn; }
		inline void setCombineMatrix2(D3DXMATRIX *matIn = NULL) { m_matCombine2 = matIn; }

		void position(float fXPos, float fYPos, float fZPos);
		void positionRelative(float fXAdd, float fYAdd, float fZAdd);

		void move(float fXPos, float fYPos, float fZPos);
		void moveRelative(float fXAdd, float fYAdd, float fZAdd);

		void rotate(float fXRot, float fYRot, float fZRot);
		void rotateRelative(float fXAdd, float fYAdd, float fZAdd);

		void scale(float fXScale, float fYScale, float fZScale);
		void scaleRelative(float fXAdd, float fYAdd, float fZAdd);

		inline float getXPos() { return m_fXPos; }
		inline float getYPos() { return m_fYPos; }
		inline float getZPos() { return m_fZPos; }
		inline float getXRotation() { return m_fXRotation; }
		inline float getYRotation() { return m_fYRotation; }
		inline float getZRotation() { return m_fZRotation; }
		inline float getXScale() { return m_fXScale; }
		inline float getYScale() { return m_fYScale; }
		inline float getZScale() { return m_fZScale; }
		inline D3DXMATRIX getMatrix() { return m_matWorld; }

	protected:
		bool m_bBillboard;
		float m_fXPos, m_fYPos, m_fZPos;
		float m_fXRotation, m_fYRotation, m_fZRotation;
		float m_fXScale, m_fYScale, m_fZScale;
		D3DXMATRIX m_matWorld;
		D3DXMATRIX m_matScale;
		D3DXMATRIX m_matRotation;
		D3DXMATRIX m_matTranslation;
		D3DXMATRIX *m_matCombine1;
		D3DXMATRIX *m_matCombine2;
	};

}