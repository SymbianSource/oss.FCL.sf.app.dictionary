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
* Description:     Japanese user dictionary Model class.
*
*/





// INCLUDE FILES
#include    "UdictModel.h"
#include    "UdictModel.inl"
#include    <PtiEngine.h>

// CONSTANTS
const TInt KItemListGranularity(8);
_LIT(KPanicCatModel, "UserDictModel");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserDictModel::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUserDictModel* CUserDictModel::NewL()
    {
    CUserDictModel* self = new(ELeave) CUserDictModel;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// Destructor
CUserDictModel::~CUserDictModel()
    {
    delete iItemList;
    delete iPtiEngine;
    }

// -----------------------------------------------------------------------------
// CUserDictModel::AddNewWordL
// -----------------------------------------------------------------------------
//    
TInt CUserDictModel::AddNewWordL(const TUDictElement& aItem)
    {
    User::LeaveIfError(IsDictionaryFull() ? KErrDiskFull : KErrNone);

    // Checks the word list if the entry is not registered to it.
    TInt index(0);
    TWordListKey key;
    if (iItemList->Count() > 0 && !iItemList->FindIsq(aItem, key, index))
        {
        User::Leave(KErrAlreadyExists);
        }

    // Adds a new entry to the User dictionary.
    User::LeaveIfError(
        iPtiEngine->AddUserDictionaryEntry(const_cast<TUDictElement&>(aItem)));
          
    // Adds a new entry to the registered word list.
    iItemList->InsertL(index, aItem);

    return index;
    }
    
// -----------------------------------------------------------------------------
// CUserDictModel::DeleteWordL
// -----------------------------------------------------------------------------
//    
void CUserDictModel::DeleteWordL(TInt aIndex)
    {
    ReloadDictInfoL();
    const TUDictElement& item = ListItem(aIndex);

    // Removes the entry from the User dictionary.
    TInt err(iPtiEngine->RemoveEntryFromUserDictionary(
                            const_cast<TUDictElement&>(item)));
    // KErrNotFound means the entry is incorrect. It should be ASSERT.
    __ASSERT_ALWAYS(err != KErrNotFound, User::Panic(KPanicCatModel, err));

    // Removes the entry from the registered word list.
    iItemList->Delete(aIndex);
    }

// -----------------------------------------------------------------------------
// CUserDictModel::ModifyWordL
// -----------------------------------------------------------------------------
//    
TInt CUserDictModel::ModifyWordL(TInt aIndex, const TUDictElement& aItem)
    {
    TInt index = AddNewWordL(aItem);
    if (index <= aIndex)
        {
        DeleteWordL(aIndex + 1);
        }
    else
        {
        DeleteWordL(aIndex);
        index--;
        }
    return index;
    }

// -----------------------------------------------------------------------------
// CUserDictModel::InitListL
// -----------------------------------------------------------------------------
//    
void CUserDictModel::InitListL()
    {
    iItemList->Reset();

    TUDictElement entry;
    TInt i(0);
    // Copies the User dictionary entries to iItemList.
    while (iPtiEngine->GetUserDictionaryEntry(i, entry) == KErrNone)
        {
        iItemList->AppendL(entry);
        i++;
        }

    // Sorts the word list by Unicode collation algorithm.
    TWordListKey key; 
    iItemList->Sort(key);
    
    SetChangeNotice(EFalse);
    }

// -----------------------------------------------------------------------------
// CUserDictModel::CUserDictModel
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUserDictModel::CUserDictModel()
    {
    }

// -----------------------------------------------------------------------------
// CUserDictModel::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUserDictModel::ConstructL()
    {
    iPtiEngine = CPtiEngine::NewL(ETrue);
    User::LeaveIfError(
        iPtiEngine->ActivateLanguageL(ELangJapanese, EPtiEngineHiraganaKanji));
    iDictionary = 
        iPtiEngine->DefaultUserDictionary(EPtiEngineHiraganaKanji);
    User::LeaveIfNull(iDictionary);

    iItemList = new (ELeave) CUserDictList(KItemListGranularity);
    }

// -----------------------------------------------------------------------------
// CUserDictModel::TWordListKey::TWordListKey
// -----------------------------------------------------------------------------
//
CUserDictModel::TWordListKey::TWordListKey()
:TKeyArrayFix(0, ECmpCollated)
    {
    }

// -----------------------------------------------------------------------------
// CUserDictModel::TWordListKey::Compare
// -----------------------------------------------------------------------------
//
TInt CUserDictModel::TWordListKey::Compare(TInt aLeft,TInt aRight) const
    {
    TUDictElement* left = static_cast<TUDictElement*>(At(aLeft));
    TUDictElement* right = static_cast<TUDictElement*>(At(aRight));
    
    TInt res(left->Reading().CompareC(right->Reading()));
    if (res == 0)
        {
        res = left->Word().CompareC(right->Word());
        }
    return res;
    }

// -----------------------------------------------------------------------------
// CUserDictModel::MdcaCount
// -----------------------------------------------------------------------------
//
TInt CUserDictModel::MdcaCount() const
    {
    return iItemList->Count();
    }

// -----------------------------------------------------------------------------
// CUserDictModel::MdcaPoint
// -----------------------------------------------------------------------------
//
TPtrC16 CUserDictModel::MdcaPoint(TInt aIndex) const
    {
    TPtrC16 ptr(iItemList->At(aIndex).ListItemText());
    return ptr;
    }

//  End of File  
