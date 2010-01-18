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
* Description:     Japanese user dictionary's editor form.
*
*/





// INCLUDE FILES
#include    "UdictFrm.h"
#include    "UdictView.h"
#include    "UdictElement.h"
#include    "UserDict.hrh"
#include    <UserDict.rsg>
#include    <avkon.hrh>
#include    <eikedwin.h>

// CONSTANTS
const TInt KNumOfInvalidFirstChar(12);
const TText KInvalidFirstChars[KNumOfInvalidFirstChar] =
    {
    0x3041, // small A
    0x3043, // small I
    0x3045, // small U
    0x3047, // small E
    0x3049, // small O
    0x3063, // small TU
    0x3083, // small YA
    0x3085, // small YU
    0x3087, // small YO
    0x3092, // WO
    0x3093, // N
    0x30fc  // prolonged sound mark
    };

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserDictForm::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUserDictForm* CUserDictForm::NewL(const TUDictElement& aItem,
                                   CUserDictView& aView)
    {
    CUserDictForm* self = new(ELeave) CUserDictForm(aItem, aView);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self

    return self;
    }

// Destructor
CUserDictForm ::~CUserDictForm ()
    {
    }

// -----------------------------------------------------------------------------
// CUserDictForm::CUserDictForm
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUserDictForm::CUserDictForm(const TUDictElement& aItem, CUserDictView& aView)
:iInitEntry(aItem), iView(aView)
    {
    }

// -----------------------------------------------------------------------------
// CUserDictForm::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUserDictForm::ConstructL()
    {
    CAknForm::ConstructL();
    }

// -----------------------------------------------------------------------------
// CUserDictForm::SaveL
// -----------------------------------------------------------------------------
//
TBool CUserDictForm::SaveL()
    {
    TBool res(ETrue);

    TUDictElement entry;
    GetData(entry);
    if (entry != iInitEntry)
        {
        if (IsValidReadingL(entry))
            {
            TRAPD(err, iView.SaveL(entry, iInitEntry));
            switch (err)
                {
                case KErrDiskFull: // No space to register
                    iView.ShowInformationNoteL(
                        R_UDICT_INFO_MEMORY_FULL_MESSAGE);
                    // if it's a new entry, returns EFalse
                    res = (iInitEntry.IsEmpty()) ? EFalse : ETrue;
                    break;
                case KErrAlreadyExists: // Already registered
                    iView.ShowInformationNoteL(R_UDICT_ALREADY_REG_MESSAGE);
                    res = EFalse;
                    break;
                default:
                    User::LeaveIfError(err);
                }
            }
        else
            {
            TryChangeFocusToL(EUserDictReadingEdwin);
            static_cast<CEikEdwin*>(
                Control(EUserDictReadingEdwin))->SetCursorPosL(0, EFalse);
            res = EFalse;
            }
        }
    return res;
    }

// -----------------------------------------------------------------------------
// CUserDictForm::GetData
// -----------------------------------------------------------------------------
//
void CUserDictForm::GetData(TUDictElement& aEntry) const
    {
    CPlainText*  text =
        static_cast<CEikEdwin*>(Control(EUserDictWordEdwin))->Text();
    TInt docLen = text->DocumentLength();
    if (docLen > EUseDictWordBuffer)
        {
        docLen = EUseDictWordBuffer;
        }
    TPtrC word(static_cast<CEikEdwin*>(
        Control(EUserDictWordEdwin))->Text()->Read(0, docLen));
    aEntry.SetWord(word);
    text = static_cast<CEikEdwin*>(Control(EUserDictReadingEdwin))->Text();
    TPtrC reading(text->Read(0, text->DocumentLength()));
    aEntry.SetReading(reading);
    }

// -----------------------------------------------------------------------------
// CUserDictForm::SetDataL
// -----------------------------------------------------------------------------
//
void CUserDictForm::SetDataL() const
    {
    TPtrC ptrWord(iInitEntry.Word());
    static_cast<CEikEdwin*>(
        Control(EUserDictWordEdwin))->SetTextL(&ptrWord);
    TPtrC ptrReading(iInitEntry.Reading());
    static_cast<CEikEdwin*>(
        Control(EUserDictReadingEdwin))->SetTextL(&ptrReading);
    }

// -----------------------------------------------------------------------------
// CUserDictForm::IsBlank
// -----------------------------------------------------------------------------
//
TBool CUserDictForm::IsBlank() const
    {
    return (((static_cast<CEikEdwin*>(
                Control(EUserDictWordEdwin))->TextLength() <= 0)
        || (static_cast<CEikEdwin*>(
                Control(EUserDictReadingEdwin))->TextLength() <= 0)
        ) ? ETrue : EFalse);
    }

// -----------------------------------------------------------------------------
// CUserDictForm::IsValidReadingL
// -----------------------------------------------------------------------------
//
TBool CUserDictForm::IsValidReadingL(const TUDictElement& aItem) const
    {
    TBool valid(ETrue);
    const TDesC& reading = aItem.Reading();
    if (reading.Length() <= 0)
        return EFalse;

    // If the reading has a invalid first character,
    // it shows the information note.
    for (TInt i(0); i < KNumOfInvalidFirstChar && valid; i++)
        {
        if (reading[0] == KInvalidFirstChars[i])
            {
            iView.ShowInformationNoteL(R_UDICT_INVALID_1STCHAR_MESSAGE);
            valid = EFalse;
            }
        }

    return valid;
    }

// -----------------------------------------------------------------------------
// CUserDictForm::PreLayoutDynInitL
// From CEikDialog
// -----------------------------------------------------------------------------
//
void CUserDictForm::PreLayoutDynInitL()
    {
    SetDataL();
    // if the form edits a new word, "Done" softkey is hidden.
    if (iInitEntry.IsEmpty())
        {
        ButtonGroupContainer().SetCommandSetL( R_UDICT_SOFTKEYS_EMPTY_CANCEL_EMPTY );
        ButtonGroupContainer().DrawNow();
        iSoftkeyResource = R_UDICT_SOFTKEYS_EMPTY_CANCEL_EMPTY;
        }
    }

// -----------------------------------------------------------------------------
// CUserDictForm::OkToExitL
// From CEikDialog
// -----------------------------------------------------------------------------
//
TBool CUserDictForm::OkToExitL(TInt aKeycode)
    {
    TBool ret = ETrue;
    switch (aKeycode)
        {
        case EAknSoftkeyDone:
            ret = SaveL();
            break;
        case EAknSoftkeyEmpty:
            ret = EFalse;
            break;
        case EAknSoftkeyCancel:
        default:
            break;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CUserDictForm::HandleControlStateChangeL
// From MCoeControlObserver
// -----------------------------------------------------------------------------
//
void CUserDictForm::HandleControlStateChangeL(TInt aControlId)
    {
    // If the entry fields are blank, "Done" softkey is hidden.
    if (aControlId == EUserDictWordEdwin
       || aControlId == EUserDictReadingEdwin)
        {
        TUint res = 0;
        if (IsBlank())
            {
            res = R_UDICT_SOFTKEYS_EMPTY_CANCEL_EMPTY;
            }
        else
            {
            res = R_UDICT_SOFTKEYS_DONE_CANCEL_DONE;
            }
        if (iSoftkeyResource != res)
            {
            ButtonGroupContainer().SetCommandSetL( res );
            ButtonGroupContainer().DrawDeferred();
            iSoftkeyResource = res;
            }
        }
    CAknForm::HandleControlStateChangeL(aControlId);
    }

// -----------------------------------------------------------------------------
// CUserDictForm::OfferKeyEventL
// From CCoeControl
// -----------------------------------------------------------------------------
//
TKeyResponse CUserDictForm::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
    {
    TKeyResponse res(EKeyWasConsumed);
    if (aType == EEventKey && aKeyEvent.iCode == EKeyOK)
        {
        if (aKeyEvent.iRepeats <= 0 && !IsBlank())
            {
            TryExitL(EAknSoftkeySelect);
            }
        }
    else
        {
        res = CAknForm::OfferKeyEventL(aKeyEvent, aType);
        }
    return res;
    }

//  End of File
