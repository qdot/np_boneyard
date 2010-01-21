/********************************************************************
	created:	2002/11/21
	created:	21:11:2002   19:45
	filename: 	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine\dynamicvb.h
	file path:	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine
	file base:	dynamicvb
	file ext:	h
	author:		qDot <qDot@numberporn.com>
	
	purpose:	Taken from the nVidia website at:

	http://developer.nvidia.com/view.asp?IO=dynamic_vb_ib

	These files handle vertex and index buffer functions.
*********************************************************************/

#ifndef _DYNAMICVB_H_
#define _DYNAMICVB_H_

#include <d3d8.h>
#include <assert.h>

/////////////////////////////
// D. Sim Dietrich Jr.
// sim.dietrich@nvidia.com
//////////////////////

template < class VertexType >
class DynamicVB 
{
	private :

		LPDIRECT3DVERTEXBUFFER8 mpVB;

		unsigned int mVertexCount;
		unsigned int mPosition;

		bool		 mbLocked;
		bool		 mbFlush;

		D3DVERTEXBUFFER_DESC mDesc;

		enum LOCK_FLAGS
		{
			LOCKFLAGS_FLUSH  = D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD,
			LOCKFLAGS_APPEND = D3DLOCK_NOSYSLOCK | D3DLOCK_NOOVERWRITE
		};

	public :

		DynamicVB( const LPDIRECT3DDEVICE8 pD3D, const DWORD& theFVF, const unsigned int& theVertexCount )
		{
			mpVB = 0;
			mPosition = 0;

			mbFlush = true;

			mbLocked = false;

			mVertexCount = theVertexCount;

		
			memset( &mDesc, 0x00, sizeof( mDesc ) );
			mDesc.Format = D3DFMT_VERTEXDATA;
			mDesc.Size = theVertexCount * sizeof( VertexType );
			mDesc.Type = D3DRTYPE_VERTEXBUFFER;
			mDesc.Pool = D3DPOOL_DEFAULT;
			mDesc.FVF = theFVF;

			mDesc.Usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;

			HRESULT hr = pD3D->CreateVertexBuffer( mVertexCount * sizeof( VertexType ),
				                                   mDesc.Usage,
												   mDesc.FVF,
												   mDesc.Pool,
												   &mpVB);
			assert( ( hr == D3D_OK ) && ( mpVB ) );
		}

		DWORD getFVF()
		{
			return mDesc.FVF;
		}

		LPDIRECT3DVERTEXBUFFER8 GetInterface() const { return mpVB; }

		// Use at beginning of frame to force a flush of VB contents on first draw
		void FlushAtFrameStart() { mbFlush = true; }


		VertexType* Lock( const unsigned int& theLockCount, unsigned int& theStartVertex )
		{
			theStartVertex = 0;
			VertexType* pLockedData = 0;

			// Ensure there is enough space in the VB for this data
			if ( theLockCount > mVertexCount ) { assert( false ); return 0; }

			if ( mpVB )
			{
				DWORD dwFlags = LOCKFLAGS_APPEND;

				// If either user forced us to flush,
				//  or there is not enough space for the vertex data,
				//  then flush the buffer contents
				//
				if ( mbFlush || ( ( theLockCount + mPosition ) > mVertexCount ) )
				{
					mbFlush = false;
					mPosition = 0;
					dwFlags = LOCKFLAGS_FLUSH;
				}

				DWORD dwSize = 0;
				HRESULT hr = mpVB->Lock( mPosition * sizeof( VertexType ), 
										 theLockCount * sizeof( VertexType ), 
										 reinterpret_cast< BYTE** >( &pLockedData ), 
										 dwFlags );

				assert( hr == D3D_OK );
				if ( hr == D3D_OK )
				{
					assert( pLockedData != 0 );
					mbLocked = true;
					theStartVertex = mPosition;
					mPosition += theLockCount;
				}
			}

			return pLockedData;
		}

		void Unlock()
		{
			if ( ( mbLocked ) && ( mpVB ) )
			{
				HRESULT hr = mpVB->Unlock();				
				assert( hr == D3D_OK );
				mbLocked = false;
			}
		}

		~DynamicVB()
		{
			Unlock();
			if ( mpVB )
			{
				mpVB->Release();
			}
		}
	
};

#endif  _DYNAMICVB_H_
