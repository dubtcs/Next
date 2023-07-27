
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
		
		*pdwEffect &= DROPEFFECT_COPY;

		return S_OK;
	}

	HRESULT DropTarget::DragLeave()
	{

		return S_OK;
	}

	HRESULT DropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{

		return S_OK;
	}

	HRESULT DropTarget::Drop(IDataObject* dataObjectPtr, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		std::cout << "Dropped file\n";
		
		FORMATETC format{ CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		STGMEDIUM medium{};

		if (SUCCEEDED(dataObjectPtr->GetData(&format, &medium)))
		{
			HDROP drop{ static_cast<HDROP>(medium.hGlobal) };
			UINT files{ DragQueryFile(drop, 0xFFFFFFFF, NULL, 0) };
			if (files == 1)
			{
				wchar_t fileSize[MAX_PATH]{};
				UINT amount{ DragQueryFile(drop, 0, fileSize, MAX_PATH) };

				events::DragFileReceived ev{ nxt::WCharPtrToString(fileSize) };
				nxt::app::OnEvent(ev);
			}
			//for (UINT i{ 0 }; i < files; i++)
			//{
			//	wchar_t fileSize[MAX_PATH]{};
			//	UINT amount{ DragQueryFile(drop, i, fileSize, MAX_PATH) };

			//	//if (amount > 0 && amount < MAX_PATH)
			//	//{
			//	//	std::cout << nxt::WCharPtrToString(fileSize);
			//	//}

			//	// only supporting 1 file transfer atm
			//	
			//}
			ReleaseStgMedium(&medium);
		}

		*pdwEffect &= DROPEFFECT_COPY;
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

/*

//IEnumFORMATETC* fmtPtr{ nullptr };
		//if (dataObjectPtr->EnumFormatEtc(DATADIR_GET, &fmtPtr) == S_OK)
		//{

		//}
		//fmtPtr->Release();
		FORMATETC request{
			CF_HDROP,
			NULL,
			DVASPECT_CONTENT,
			-1,
			TYMED_FILE
		};
		FORMATETC receive{};
		HRESULT status{ dataObjectPtr->GetCanonicalFormatEtc(&request, &receive) };

		// Currently getting invalid DVTARGETDEVICE error
		if (status == DATA_S_SAMEFORMATETC)
		{
			STGMEDIUM objMedium{};
			HRESULT getStatus{ dataObjectPtr->GetData(&receive, &objMedium) };
			std::cout << getStatus << "\n";
			if (getStatus == S_OK)
			{
				std::cout << "OK Data received\n";
				if (objMedium.pUnkForRelease == NULL)
				{
					std::cout << "PANICKING\n";
					ReleaseStgMedium(&objMedium);
				}
				else
				{
					std::cout << "We good.\n";
				}
			}
		}
*/
