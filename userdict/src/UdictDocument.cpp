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
* Description:     Japanese user dictionary Document class.
*
*/






// INCLUDE FILES
#include "UdictDocument.h"
#include "UdictAppUi.h"
#include "UdictModel.h"
#include <AknDoc.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserDictDocument::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUserDictDocument* CUserDictDocument::NewL(
        CEikApplication& aApp)     // CUserDictApp reference
    {
    CUserDictDocument* self = new (ELeave) CUserDictDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self

    return self;
    }

// Destructor
CUserDictDocument::~CUserDictDocument()
    {
    delete iModel;
    }

// -----------------------------------------------------------------------------
// CUserDictDocument::CUserDictDocument
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUserDictDocument::CUserDictDocument(CEikApplication& aApp)
: CAknDocument(aApp)    
    {
    }

// -----------------------------------------------------------------------------
// CUserDictDocument::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUserDictDocument::ConstructL()
    {
    iModel = CUserDictModel::NewL();
    }

// -----------------------------------------------------------------------------
// CUserDictDocument::CreateAppUiL()
// Constructs CUserDictAppUi
// -----------------------------------------------------------------------------
//
CEikAppUi* CUserDictDocument::CreateAppUiL()
    {
    return new (ELeave) CUserDictAppUi();
    }

// -----------------------------------------------------------------------------
// CUserDictDocument::Model()
// Returns CUserDictModel object.
// -----------------------------------------------------------------------------
//
CUserDictModel& CUserDictDocument::Model() const
    {
    return *iModel;
    }

//  End of File  
