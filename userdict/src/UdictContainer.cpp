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
* Description:     Japanese user dictionary View Container control.
*
*/





// INCLUDE FILES
#include "UdictContainer.h"
#include "UdictContainer.inl"
#include "UdictView.h"
#include "UdictModel.h"
#include "UdictModel.inl"
#include "UdictApp.h"
#include "UserDict.hrh"
#include "udict.hlp.hrh"
#include <userdict.rsg>
#include <aknconsts.h>
#include <aknlists.h>
#include <eikclbd.h>
#include <AknIconArray.h>
#include <avkon.mbg>
#include <AknWaitNoteWrapper.h>
#include <eikmenub.h>
#include <gulicon.h>
#include <AknIconUtils.h>

// CONSTANTS
const TInt KGulIconGranurality(10);
const TInt KWaitNoteOneStepDeletion(20);

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserDictContainer::CUserDictContainer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUserDictContainer::CUserDictContainer(
                    CUserDictModel& aModel,
                    CAknView& aView)
:iModel(aModel), iView(aView), iWaitNoteCounter(-1)
    {
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUserDictContainer::ConstructL()
    {
    CreateWindowL();

    iListBox = new (ELeave) CAknSingleStyleListBox;
    iListBox->SetContainerWindowL(*this);
    iListBox->ConstructL(this, EAknListBoxMarkableList);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
    iModel.InitListL();
    iListBox->Model()->SetItemTextArray(iModel.List());
    iListBox->Model()->SetOwnershipType(ELbmDoesNotOwnItemArray);
    SetGraphicIconL();

    // Here is NOT call ActivateL(), because the client size isn't set yet.
    // The caller of this ConstructL() must call ActivateL().
    }

// Destructor
CUserDictContainer::~CUserDictContainer()
    {
    delete iListBox;
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::SetFocusL
// -----------------------------------------------------------------------------
//
void CUserDictContainer::SetFocusL(TInt aIndex) const
    {
    AknListBoxUtils::HandleItemRemovalAndPositionHighlightL(
        iListBox, aIndex, EFalse);
    DrawNow();
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::DeleteItemL
// -----------------------------------------------------------------------------
//
void CUserDictContainer::DeleteItemL()
    {
    TInt count = iListBox->View()->SelectionIndexes()->Count();
    TInt index(iListBox->CurrentItemIndex());
    if (count > 0)
        {
        const CListBoxView::CSelectionIndexArray* selectionIndexes =
           iListBox->View()->SelectionIndexes();
        TKeyArrayFix key(0, ECmpTInt);
        const_cast<CListBoxView::CSelectionIndexArray*>(
                                selectionIndexes)->Sort(key);
        iWaitNoteCounter = --count;
        ShowWaitNoteL();
        TInt selectionIndex;
        selectionIndexes->FindIsq(index, key, selectionIndex);
        // It sets the focused item index.
        // If the focused item has been removed, the focus goes to the next
        // item. Otherwise, the focus stays in there.
        SetFocusL(index - selectionIndex);
        }
    else
        {
        iModel.DeleteWordL(index);
        AknListBoxUtils::HandleItemRemovalAndPositionHighlightL(
            iListBox, index, ETrue);
        DrawNow();
        }
    }

// -----------------------------------------------------------------------------
// void CUserDictContainer::SetGraphicIconL
// -----------------------------------------------------------------------------
//
void CUserDictContainer::SetGraphicIconL()
    {
    // Creates gul icon.
    CArrayPtr<CGulIcon>* icons = new( ELeave ) CAknIconArray(
                                  KGulIconGranurality );
    CleanupStack::PushL(icons);

    // Appends icon.
    CreateIconAndAddToArrayL(icons, KAvkonBitmapFile,
        EMbmAvkonQgn_indi_marked_add, EMbmAvkonQgn_indi_marked_add_mask);

    // Sets graphics as ListBox icon.
    iListBox->ItemDrawer()->ColumnData()->SetIconArray(icons);

    CleanupStack::Pop(); // icons
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::ShowWaitNoteL
// -----------------------------------------------------------------------------
//
void CUserDictContainer::ShowWaitNoteL()
    {
    CAknWaitNoteWrapper* note = CAknWaitNoteWrapper::NewL();
    iModel.DelaySavingL();
    TRAPD(err, note->ExecuteL(R_UDICT_WAIT_NOTE, *this));
    delete note;
    iModel.SaveNowL();
    iWaitNoteCounter = -1;
    User::LeaveIfError(err);
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::CreateIconAndAddToArrayL
// Creates the icon and adds it to the array if it was successful
// -----------------------------------------------------------------------------
void CUserDictContainer::CreateIconAndAddToArrayL(CArrayPtr<CGulIcon>*& aIconsArray,
                                               const TDesC& aIconFile,
                                               TInt aBitmap, TInt aMask/* = -1*/)
    {
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    if (aMask >= 0)
        {
        AknIconUtils::CreateIconL( bitmap, mask, aIconFile, aBitmap, aMask);
        }
    else
        {
        bitmap = AknIconUtils::CreateIconL( aIconFile, aBitmap);
        }

    if (bitmap)
        {
        icon->SetBitmap(bitmap);
        if (mask)
            icon->SetMask(mask);
        aIconsArray->AppendL(icon);
        CleanupStack::Pop(icon);
        }
    else
        {
        CleanupStack::PopAndDestroy(icon);
        }
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::SizeChanged()
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CUserDictContainer::SizeChanged()
    {
    iListBox->SetRect(Rect());
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::CountComponentControls
// From CCoeControl
// -----------------------------------------------------------------------------
//
TInt CUserDictContainer::CountComponentControls() const
    {
    return 1; // return a number of controls inside this container
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::ComponentControl
// From CCoeControl
// -----------------------------------------------------------------------------
//
CCoeControl* CUserDictContainer::ComponentControl(TInt aIndex) const
    {
    switch (aIndex)
        {
        case 0:
            return iListBox;
        default:
            return NULL;
        }
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::OfferKeyEventL
// From CCoeControl
// -----------------------------------------------------------------------------
//
TKeyResponse CUserDictContainer::OfferKeyEventL(
                const TKeyEvent& aKeyEvent, TEventCode aType)
    {
    TKeyResponse res(EKeyWasNotConsumed);

    if (aType == EEventKeyDown
        && IsFocused())
        {
        // If user pushes some digit key, it starts the User dictionary editor.
        TChar key(aKeyEvent.iScanCode);
        if (key.IsDigit())
            {
            iView.HandleCommandL(EUserDictCmdNewWord);
            res = EKeyWasConsumed;
            }
        }
    else if (aType == EEventKey)
        {
        switch (aKeyEvent.iCode)
            {
            case EKeyBackspace:
                if (iModel.List()->MdcaCount() > 0)
                    {
                    iView.HandleCommandL(EUserDictCmdDelete);
                    res = EKeyWasConsumed;
                    }
                break;
            case EKeyOK:
                if (iModel.List()->MdcaCount() <= 0
                    || iListBox->View()->SelectionIndexes()->Count() > 0)
                    {
                    // It shows context sensitive menu.
                    CEikMenuBar* menuBar = iView.MenuBar();
                    menuBar->SetMenuTitleResourceId(
                                R_UDICT_MENUBAR_VIEW_CONTEXT);
                    menuBar->TryDisplayMenuBarL();
                    menuBar->SetMenuTitleResourceId(
                                R_UDICT_MENUBAR_VIEW);
                    }
                else
                    {
                    iView.HandleCommandL(EUserDictCmdOpen);
                    }
                res = EKeyWasConsumed;
                break;
            default:
                break;
            }
        }

    if (res != EKeyWasConsumed && iListBox)
        {
        res = iListBox->OfferKeyEventL(aKeyEvent, aType);
        }

    return res;
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::GetHelpContext
// -----------------------------------------------------------------------------
//
void CUserDictContainer::GetHelpContext(TCoeHelpContext& aContext) const
    {
    aContext.iMajor = KUidUserDict;
    aContext.iContext = KUDICT_HLP;
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::StepL
// From MAknBackgroundProcess
// -----------------------------------------------------------------------------
void CUserDictContainer::StepL()
    {
    // Deletes the selected item in descending order of list index.
    for (TInt i(0); i < KWaitNoteOneStepDeletion && iWaitNoteCounter >= 0; i++)
        {
        iModel.DeleteWordL(
            (*iListBox->View()->SelectionIndexes())[iWaitNoteCounter]);
        iWaitNoteCounter--;
        }
    }

// -----------------------------------------------------------------------------
// CUserDictContainer::IsProcessDone
// From MAknBackgroundProcess
// Tests if wait note process is done.
// -----------------------------------------------------------------------------
TBool CUserDictContainer::IsProcessDone() const
    {
    return (iWaitNoteCounter < 0) ? ETrue : EFalse;
    }

//  End of File
