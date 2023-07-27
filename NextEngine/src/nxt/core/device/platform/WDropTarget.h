#pragma once

#include <nxt/EngineCore.h>
#include <Windows.h>

namespace nxt::device
{

	class DropTarget : public IDropTarget
	{
	public:
		DropTarget();
		virtual HRESULT DragEnter(IDataObject* dataObjectPtr, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
		virtual HRESULT DragLeave() override;
		virtual HRESULT DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
		virtual HRESULT Drop(IDataObject* dataObjectPtr, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;

		virtual HRESULT QueryInterface(REFIID riid, void** pp) override;
		virtual ULONG AddRef() override;
		virtual ULONG Release() override;
	};

}
