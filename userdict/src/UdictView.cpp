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
* Description:     Japanese user dictionary View class.
*
*/





// INCLUDE FILES
#include  <bldvariant.hrh>
#include  "UdictApp.h"
#include  "UdictView.h"
#include  "UdictModel.h"
#include  "UdictModel.inl"
#include  "UdictContainer.h"
#include  "UdictContainer.inl"
#include  "UdictFrm.h"
#include  "UserDict.hrh"
#include  <userdict.rsg>
#include  <aknViewAppUi.h>
#include  <aknmessagequerydialog.h>
#include  <StringLoader.h>
#include  <eikmenub.h>
#include  <featmgr.h>
#include  <hlplch.h>
#include  <aknnotewrappers.h>
#include <w32std.h>

// CONSTANTS
const TUid KViewId = {1}; // UID of View
const TInt KMessageBuffSize(64);
#if defined(_DEBUG)
_LIT(KPanicCatView, "UserDictView");
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserDictView::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUserDictView* CUserDictView::NewLC(CUserDictModel& aModel)
    {
    CUserDictView* self = new (ELeave) CUserDictView(aModel);
    CleanupStack::PushL(self);
    self->ConstructL();

    return self;
    }

// Destructor
CUserDictView::~CUserDictView()
    {
    if (iContainer)
        {
        AppUi()->RemoveFromViewStack(*this, iContainer);
        delete iContainer;
        }
    delete iIdle;
    }

// -----------------------------------------------------------------------------
// CUserDictView::SaveL
// Saves the changes of the element and updates ListBox.
// -----------------------------------------------------------------------------
//
void CUserDictView::SaveL(
    const TUDictElement& aNewItem, const TUDictElement& aOldItem) const
    {
#if defined(_DEBUG)
    __ASSERT_DEBUG(!aNewItem.IsEmpty(),
        User::Panic(KPanicCatView, KErrCorrupt));
#endif
    TInt index(iContainer ? iContainer->ListBox()->CurrentItemIndex() : 0);
    if (aOldItem.IsEmpty())
        {
        index = iModel.AddNewWordL(aNewItem);
        }
    else
        {
        index = iModel.ModifyWordL(index, aNewItem);
        }

#ifdef RD_PF_SEC_APPARC
    if (iEmbedded)
#else
    if (AppUi()->ContainerAppUi())
#endif
        {
        iModel.SetChangeNotice(ETrue);
        }

    if (iContainer)
        {
        iContainer->SetFocusL(index);
        }
    }

// -----------------------------------------------------------------------------
// CUserDictView::ShowInformationNoteL
// -----------------------------------------------------------------------------
//
void CUserDictView::ShowInformationNoteL(TInt aResourceId) const
    {
    TBuf<KMessageBuffSize> message;
    iCoeEnv->ReadResourceL(message, aResourceId);
    CAknInformationNote* note = new(ELeave) CAknInformationNote();
    note->ExecuteLD(message);
    }

// -----------------------------------------------------------------------------
// CUserDictView::CUserDictView
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUserDictView::CUserDictView(CUserDictModel& aModel)
:iModel(aModel)
    {
    }

// -----------------------------------------------------------------------------
// CUserDictView::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUserDictView::ConstructL()
    {
    BaseConstructL(R_UDICT_VIEW);
    }

// -----------------------------------------------------------------------------
// CUserDictView::ShowDeleteConfQueryL
// Shows delete confirmation query.
// -----------------------------------------------------------------------------
//
TInt CUserDictView::ShowDeleteConfQueryL(TInt aCount) const
    {
    TInt ret(0);
    if (aCount <= 1)
        {
        CAknQueryDialog* dialog = CAknQueryDialog::NewL();
        ret = dialog->ExecuteLD(R_UDICT_DELETE_QUERY);
        }
    else
        {
        HBufC* message = StringLoader::LoadL(
            R_UDICT_DELETE_QUERY_MESSAGE, aCount, iCoeEnv);
        CleanupStack::PushL(message);
        CAknQueryDialog* dialog = CAknQueryDialog::NewL();
        ret = dialog->ExecuteLD(R_UDICT_DELETE_QUERY, *message);
        CleanupStack::PopAndDestroy(); // message
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CUserDictView::ShowEditorFormL
// Shows user dictionary editor form.
// -----------------------------------------------------------------------------
//
void CUserDictView::ShowEditorFormL(const TUDictElement& item)
    {
    CUserDictForm* form = CUserDictForm::NewL(item, *this);
    form->ExecuteLD(R_UDICT_DIALOG_EDIT);
    }

// -----------------------------------------------------------------------------
// CUserDictView::UpdateListL
// -----------------------------------------------------------------------------
//
void CUserDictView::UpdateListL()
    {
    iModel.InitListL();
    TInt index(iContainer->ListBox()->CurrentItemIndex());
    iContainer->SetFocusL(
        (iModel.List()->MdcaCount() > 0 && index < 0) ? 0 : index);
    }

// -----------------------------------------------------------------------------
// CUserDictView::RegisterByAnotherApp
// iIdle's call back function
// -----------------------------------------------------------------------------
//
TInt CUserDictView::RegisterByAnotherApp(TAny* aView)
    {
    static_cast<CUserDictView*>(aView)->DoRegisterByAnotherApp();
    return 0;
    }

// -----------------------------------------------------------------------------
// CUserDictView::DoRegisterByAnotherApp
// -----------------------------------------------------------------------------
//
void CUserDictView::DoRegisterByAnotherApp()
    {
    iIdle->Cancel();

    // If HandleCommandL() leaves, this application can't exit.
    // So, it needs to be trapped.
    TRAP_IGNORE(HandleCommandL(EUserDictCmdNewWord));

#ifdef RD_PF_SEC_APPARC
    // In case RD_PF_SEC_APPARC, CEikAppui::Exit()
    // CEikAppui::Exit() always leave :Leave code = KLeaveExit(-1003)
    // KLeaveExit should be not trapped. So Exit() is called directly.
    AppUi()->Exit();
#else
    // CEikAppui::Exit() must leave, at that time,
    // this iIdle has deleted already.
    // So, it needs to be trapped.
    TRAP_IGNORE(AppUi()->HandleCommandL(EEikCmdExit));
#endif
    }

// -----------------------------------------------------------------------------
// TUid CUserDictView::Id
// From CAknView
// -----------------------------------------------------------------------------
//
TUid CUserDictView::Id() const
    {
    return KViewId;
    }

// -----------------------------------------------------------------------------
// CUserDictView::HandleCommandL
// From CAknView
// -----------------------------------------------------------------------------
//
void CUserDictView::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        case EAknSoftkeyExit:
            AppUi()->HandleCommandL(EEikCmdExit);
            break;
        case EUserDictCmdOpen:
            {
            if (iModel.List()->MdcaCount() <= 0
                || iContainer->ListBox()->View()->SelectionIndexes()->Count() > 0)
                {
                // It shows context sensitive menu.
                CEikMenuBar* menuBar = MenuBar();
                menuBar->SetMenuTitleResourceId(
                            R_UDICT_MENUBAR_VIEW_CONTEXT);
                menuBar->TryDisplayMenuBarL();
                menuBar->SetMenuTitleResourceId(
                            R_UDICT_MENUBAR_VIEW);
                }
            else
                {
                TUDictElement item(iModel.ListItem(
                        iContainer->ListBox()->CurrentItemIndex()));
                ShowEditorFormL(item);
                // Show correct softkey
                ShowSoftkeyL();
                }
            }
            break;
        case EUserDictCmdNewWord:
            if (!iModel.IsDictionaryFull())
                {
                TUDictElement item;
                ShowEditorFormL(item);
                // Show correct softkey
                ShowSoftkeyL();
                }
            else
                {
                ShowInformationNoteL(R_UDICT_INFO_MEMORY_FULL_MESSAGE);
                }
            break;
        case EUserDictCmdDelete:
             // Forbids the list update in HandleForegroundL while deleting.
            iLockListUpdate = ETrue;
            if (ShowDeleteConfQueryL(
                iContainer->ListBox()->View()->SelectionIndexes()->Count()))
                {
                iContainer->DeleteItemL();
                }
            iLockListUpdate = EFalse;
            if (iModel.IsChangedByAnotherApp())
                {
                UpdateListL();
                }
            // Show correct softkey
            ShowSoftkeyL();
            break;
        case EAknCmdHelp:
            HlpLauncher::LaunchHelpApplicationL(
                iCoeEnv->WsSession(), AppUi()->AppHelpContextL());
            break;
        default:
		    if (iContainer && iModel.List()->MdcaCount() > 0)
		        {
				TInt previous = iContainer->ListBox()->View()->SelectionIndexes()->Count();
		        AknSelectionService::HandleMarkableListProcessCommandL(
		                                aCommand, iContainer->ListBox());
				if (previous != iContainer->ListBox()->View()->SelectionIndexes()->Count())
					{
		            // Show correct softkey
		            ShowSoftkeyL();
					}
		        }
            AppUi()->HandleCommandL(aCommand);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CUserDictView::HandleClientRectChange
// From CAknView
// -----------------------------------------------------------------------------
//
void CUserDictView::HandleClientRectChange()
    {
    if (iContainer)
        {
        iContainer->SetRect(ClientRect());
        }
    }

// -----------------------------------------------------------------------------
// CUserDictView::DoActivateL
// From CAknView
// -----------------------------------------------------------------------------
//
void CUserDictView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
#ifdef RD_PF_SEC_APPARC

    iEmbedded = EFalse;

    RArray<RWsSession::TWindowGroupChainInfo> wgs;
    CleanupClosePushL( wgs );
    TInt err = iEikonEnv->WsSession().WindowGroupList(&wgs);
    if (err == KErrNone)
        {
        TInt wgId = iEikonEnv->RootWin().Identifier();
        TInt count = wgs.Count();
        for (TInt ii=0; ii < count; ii++)
            {
            const RWsSession::TWindowGroupChainInfo& info = wgs[ii];
            // find the window group id and check that it has no parent
            if (info.iId == wgId)
                {
                if (info.iParentId > 0)
                    {
                    iEmbedded = ETrue;
                    }
                break;
                }
            }
        }
    wgs.Reset();
    CleanupStack::PopAndDestroy( &wgs );

    if (!iEmbedded)
        {
        // I'm standalone application
        iContainer = new(ELeave) CUserDictContainer(iModel, *this);
        iContainer->SetMopParent(this);
        iContainer->ConstructL();
        iContainer->SetRect(ClientRect());
        iContainer->ActivateL();

        AppUi()->AddToStackL(*this, iContainer);
        // Set context menu
        MenuBar()->SetContextMenuTitleResourceId
            ( R_UDICT_MENUBAR_VIEW_CONTEXT );
        }
    else
        {
        // embedded using window group chaining
        iIdle = CIdle::NewL(CActive::EPriorityIdle);
        // It calls CUserDictView::RegisterByAnotherApp()
        // after the activation of View finishes.
        iIdle->Start(TCallBack(RegisterByAnotherApp, this));
        }

#else

    if (AppUi()->ContainerAppUi()) // started up by another application.
        {
        iIdle = CIdle::NewL(CActive::EPriorityIdle);
        // It calls CUserDictView::RegisterByAnotherApp()
        // after the activation of View finishes.
        iIdle->Start(TCallBack(RegisterByAnotherApp, this));
        }
    else if (!iContainer)
        {
        iContainer = new(ELeave) CUserDictContainer(iModel, *this);
        iContainer->SetMopParent(this);
        iContainer->ConstructL();
        iContainer->SetRect(ClientRect());
        iContainer->ActivateL();

        AppUi()->AddToStackL(*this, iContainer);
        }
#endif //RD_PF_SEC_APPARC

   }

// -----------------------------------------------------------------------------
// CUserDictView::DoDeactivate
// From CAknView
// -----------------------------------------------------------------------------
//
void CUserDictView::DoDeactivate()
    {
    if (iContainer)
        {
        AppUi()->RemoveFromViewStack(*this, iContainer);
        delete iContainer;
        iContainer = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CUserDictView::HandleForegroundEventL
// From CAknView
// -----------------------------------------------------------------------------
//
void CUserDictView::HandleForegroundEventL(TBool aForeground)
    {
    CAknView::HandleForegroundEventL(aForeground);

    if (aForeground)
        {
        iModel.ReloadDictInfoL();

#ifdef RD_PF_SEC_APPARC
        if (!iEmbedded
            && iModel.IsChangedByAnotherApp()
            && !iLockListUpdate)
#else
        if (!AppUi()->ContainerAppUi()
            && iModel.IsChangedByAnotherApp()
            && !iLockListUpdate)
#endif
            {
            // When the User dictionary data has been changed while the
            // application was running on background, it updates the ListBox.
            UpdateListL();
            }
        // Show correct softkey
        ShowSoftkeyL();
        }
    else if (iContainer && iContainer->IsFocused())
        {
        iLockListUpdate = EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CUserDictView::DynInitMenuPaneL
// From MEikMenuObserver
// -----------------------------------------------------------------------------
//
void CUserDictView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenu)
    {
    CAknView::DynInitMenuPaneL(aResourceId, aMenu);
    if (iContainer)
        {
        AknSelectionService::HandleMarkableListDynInitMenuPane(
                    aResourceId, aMenu, iContainer->ListBox());
        }
    if (aResourceId == R_UDICT_APP_MENU)
        {
        if (iModel.List()->MdcaCount() == 0)
            {
            aMenu->DeleteMenuItem(EUserDictCmdOpen);
            aMenu->DeleteMenuItem(EUserDictCmdDelete);
            }
        if (!FeatureManager::FeatureSupported(KFeatureIdHelp))
            {
            aMenu->DeleteMenuItem(EAknCmdHelp);
            }
        }
    else if (aResourceId == R_UDICT_APP_MENU_CONTEXT)
        {
        if (iModel.List()->MdcaCount() == 0)
            {
            aMenu->DeleteMenuItem(EUserDictCmdDelete);
            }
        else
            {
            aMenu->DeleteMenuItem(EUserDictCmdNewWord);
            }
        }
    }

// -----------------------------------------------------------------------------
// CUserDictView::ShowSoftkeyL
// Show softkeys
// -----------------------------------------------------------------------------
//
void CUserDictView::ShowSoftkeyL()
    {
#ifdef RD_PF_SEC_APPARC
    if (iEmbedded)
#else
    if (AppUi()->ContainerAppUi())
#endif
        {
        Cba()->SetCommandSetL(R_UDICT_VIEW_CBA_SOFTKEY_EMPTY);
        }
    else
        {
        if (iModel.List()->MdcaCount() > 0)
            {
            if (iContainer->ListBox()->View()->SelectionIndexes()->Count() > 0)
                {
                // There is(are) the selected item(s)
                Cba()->SetCommandSetL(R_UDICT_VIEW_CBA_SOFTKEY_CONTEXTOPTIONS);
                }
            else
                {
                // There is no the selected item(s)
                Cba()->SetCommandSetL(R_UDICT_VIEW_CBA_SOFTKEY_OPEN);
                }
            }
        else
            {
            Cba()->SetCommandSetL(R_UDICT_VIEW_CBA_SOFTKEY_NEWWORD);
            }
        }
    Cba()->DrawNow();
    }

//  End of File
