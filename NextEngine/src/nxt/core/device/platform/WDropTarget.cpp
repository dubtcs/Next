
#include "../Device.h"
#include "WDropTarget.h"

#include <nxt/core/app/App.h>
#include <nxt/core/utility/TypeConversions.h>

#include <iostream>

/*

Today I will learn DropTargets


.. Damn DropTargets got hands...

*/

namespace nxt::device
{

	HRESULT DropTarget::DragEnter(IDataObject* dataObjectPtr, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		FORMATETC format{ CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

		if (SUCCEEDED(dataObjectPtr->GetData(&format, &mMedium)))
		{
			HDROP drop{ static_cast<HDROP>(mMedium.hGlobal) };
			UINT files{ DragQueryFile(drop, 0xFFFFFFFF, NULL, 0) };
			if (files == 1)
			{
				wchar_t fileSize[MAX_PATH]{};
				UINT amount{ DragQueryFile(drop, 0, fileSize, MAX_PATH) };
				std::string extension{ std::filesystem::path{ nxt::ToString(fileSize) }.extension().string() };
				if (extension == ".gltf" || extension == ".glb")
				{
					*pdwEffect &= DROPEFFECT_COPY;
					mUsable = true;
				}
			}
			//ReleaseStgMedium(&medium);
		}

		if (!mUsable)
			*pdwEffect &= DROPEFFECT_NONE;
		return S_OK;
	}

	HRESULT DropTarget::DragLeave()
	{
		mUsable = false;
		return S_OK;
	}

	HRESULT DropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		*pdwEffect &= (mUsable) ? DROPEFFECT_COPY : DROPEFFECT_NONE;
		return S_OK;
	}

	HRESULT DropTarget::Drop(IDataObject* dataObjectPtr, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		if (mUsable)
		{
			HDROP drop{ static_cast<HDROP>(mMedium.hGlobal) };
			UINT files{ DragQueryFile(drop, 0xFFFFFFFF, NULL, 0) };
			if (files == 1)
			{
				wchar_t fileSize[MAX_PATH]{};
				UINT amount{ DragQueryFile(drop, 0, fileSize, MAX_PATH) };

				events::DragFileReceived ev{ nxt::ToString(fileSize) };
				nxt::app::OnEvent(ev);
			}
			ReleaseStgMedium(&mMedium);
			*pdwEffect &= DROPEFFECT_COPY;
		}

		dataObjectPtr->Release();
		return S_OK;
	}

	HRESULT DropTarget::QueryInterface(REFIID riid, void** pp)
	{

		return S_OK;
	}

	ULONG DropTarget::AddRef()
	{

		return 1;
	}

	ULONG DropTarget::Release()
	{

		return 0;
	}

	DropTarget::DropTarget()
	{

	}
	
}
