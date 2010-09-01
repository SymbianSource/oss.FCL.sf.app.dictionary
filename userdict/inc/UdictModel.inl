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
* Description:     Inline functions of Japanese user dictionary Model class.
*
*/





// INCLUDE FILES
#include    <PtiCore.h>
#include    <PtiEngine.h>

// CONSTANTS
const TInt KRegisterNecessarySpace(75);

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserDictModel::List
// Returns "this" pointer as interface to the registered word list.
// -----------------------------------------------------------------------------
//    
inline MDesCArray* CUserDictModel::List()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// CUserDictModel::ListItem
// -----------------------------------------------------------------------------
//    
inline const TUDictElement& CUserDictModel::ListItem(TInt aIndex) const
    {
    return iItemList->At(aIndex);
    }

// -----------------------------------------------------------------------------
// CUserDictModel::IsChangedByAnotherApp
// -----------------------------------------------------------------------------
//
inline TBool CUserDictModel::IsChangedByAnotherApp() const
    {
    return iDictionary->UpdateFlag()
        && iPtiEngine->NumberOfEntriesInUserDictionary() != iItemList->Count();
    }

// -----------------------------------------------------------------------------
// CUserDictModel::SetChangeNotice
// -----------------------------------------------------------------------------
//
inline void CUserDictModel::SetChangeNotice(TBool aFlag)
    {
    iDictionary->SetUpdateFlag(aFlag);
    }

// -----------------------------------------------------------------------------
// CUserDictModel::IsDictionaryFull
// -----------------------------------------------------------------------------
//
inline TBool CUserDictModel::IsDictionaryFull()
    {
    // Checks the left space of the User dictionary data area.
    TInt leftSpace(iDictionary->SpaceLeft());
    return (leftSpace >= 0 && leftSpace < KRegisterNecessarySpace) 
           ? ETrue : EFalse;
    }

// -----------------------------------------------------------------------------
// CUserDictModel::ReloadDictInfoL
// -----------------------------------------------------------------------------
//
inline void CUserDictModel::ReloadDictInfoL()
    {
    iPtiEngine->HandleCommandL(EPtiCommandReloadLearningInfo);
    }

// -----------------------------------------------------------------------------
// CUserDictModel::DelaySavingL
// -----------------------------------------------------------------------------
//
inline void CUserDictModel::DelaySavingL()
    {
    iPtiEngine->HandleCommandL(EPtiCommandUserDictionaryDelaySaving);
    }

// -----------------------------------------------------------------------------
// CUserDictModel::SaveNowL
// -----------------------------------------------------------------------------
//
inline void CUserDictModel::SaveNowL()
    {
    iPtiEngine->HandleCommandL(EPtiCommandUserDictionarySaveNow);
    }

//  End of File  
