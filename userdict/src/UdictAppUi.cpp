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
* Description:     Japanese user dictionary Application UI class.
*
*/






// INCLUDE FILES
#include    "UdictAppUi.h"
#include    "UdictView.h"
#include    "UdictDocument.h"
#include    "UdictModel.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserDictAppUi::CUserDictAppUi
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUserDictAppUi::CUserDictAppUi()
    {
    }

// Destructor
CUserDictAppUi::~CUserDictAppUi()
    {
    }

// -----------------------------------------------------------------------------
// CUserDictAppUi::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUserDictAppUi::ConstructL()
    {
    BaseConstructL(EAknEnableSkin | EAknEnableMSK);
    CUserDictView* view = CUserDictView::NewLC(
        static_cast<CUserDictDocument*>(Document())->Model());

    AddViewL(view);      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view

    SetDefaultViewL(*view);
    }

// -----------------------------------------------------------------------------
// CUserDictAppUi::HandleCommandL
// -----------------------------------------------------------------------------
//
void CUserDictAppUi::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        case EEikCmdExit:
            Exit();
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CUserDictAppUi::HandleScreenDeviceChangedL
// -----------------------------------------------------------------------------
//
void CUserDictAppUi::HandleScreenDeviceChangedL()
    {
    CAknViewAppUi::HandleScreenDeviceChangedL();
    CEikAppUi::HandleScreenDeviceChangedL();
    const TUid KViewId = {1}; // UID of View
    CAknView* view = View(KViewId);
    if (view)
        {

        static_cast<CUserDictView*>(view)->HandleClientRectChange();
        }
    }

//  End of File
