/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:     Japanese user dictionary Application class.
*
*/






// INCLUDE FILES
#include "UdictApp.h"
#include "UdictDocument.h"
#include <ecom.h>
#include <implementationproxy.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserDictApp::AppDllUid
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CUserDictApp::AppDllUid() const
    {
    return KUidUserDict;
    }

// -----------------------------------------------------------------------------
// CUserDictApp::CreateDocumentL
// Creates CUserDictDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CUserDictApp::CreateDocumentL()
    {
    return CUserDictDocument::NewL(*this);
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// NewApplication
// Constructs CUserDictApp
// -----------------------------------------------------------------------------
//
LOCAL_C CApaApplication* NewApplication()
    {
    return new CUserDictApp;
    }

// -----------------------------------------------------------------------------
// E32Dll 
// Entry point function for SymbianOS ECom plugin
// Returns: KErrNone: No error
// -----------------------------------------------------------------------------
//
#ifndef EKA2
GLDEF_C TInt E32Dll()
    {
    return KErrNone;
    }
#endif

LOCAL_D const TImplementationProxy ImplementationTable[]=
    {
    IMPLEMENTATION_PROXY_ENTRY(0x101F8645, NewApplication)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount=sizeof(ImplementationTable)/sizeof(ImplementationTable[0]);
    return ImplementationTable;
    }


//  End of File  
