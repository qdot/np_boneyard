#include <qEngine/qCommonInclude.h>
#include <atlcomcli.h>

namespace qEngine
{
	template <typename T>
	class qDXBuffer : public CComPtr<ID3DXBuffer>
	{
	public:
		template <typename T>
		qDXBuffer<T>() : CComPtr<ID3DXBuffer>() {}

		template <typename T>
		qDXBuffer<T>(UINT count) : CComPtr<ID3DXBuffer>()
		{
			THR(::D3DXCreateBuffer(count*sizeof(T),
				CComPtr<ID3DXBuffer>::operator&()));
		}

		template <typename T>
		operator const T *() const
		{
			ATLASSERT(p);
			return static_cast<T *>(p->GetBufferPointer());
		}

		template <typename T>
		operator T *()
		{
			ATLASSERT(p);
			return static_cast<T *>(p->GetBufferPointer());
		}

		template <typename T>
		const T &operator [](UINT idx) const
		{
			ATLASSERT(p);
			return static_cast<T *>(p->GetBufferPointer())[idx];
		}

		template <typename T>
		T operator [](UINT idx)
		{
			ATLASSERT(p);
			return static_cast<T *>(p->GetBufferPointer())[idx];
		}
	};
}