//===================================================================================
// Copyright (c) Microsoft Corporation.  All rights reserved.                        
//                                                                                   
// THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY                    
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT                       
// LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND                          
// FITNESS FOR A PARTICULAR PURPOSE.                                                 
//===================================================================================


#pragma once
#include <wrl/client.h>
#include <assert.h>

template<class T>
HRESULT AssignToOutputPointer(T** pp, const Microsoft::WRL::ComPtr<T> &p)
{
	assert(pp);
	*pp = p.Get();
	if (nullptr != (*pp))
	{
		(*pp)->AddRef();
	}

	return S_OK;
}