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
*                It owns CUserDictContainer object as container control.
*                It handles the events and switches the view state to Word list
*                view or to User dictionary editor.
*
*/





#ifndef UDICTVIEW_H
#define UDICTVIEW_H

//  INCLUDES
#include <aknview.h>

// FORWARD DECLARATIONS
class CUserDictContainer;
class CUserDictModel;
class TUDictElement;
class CIdle;

// CLASS DECLARATION

/**
*  Japanese user dictionary View class.
*  It owns CUserDictContainer object as container control.
*  @since 2.6
*/
class CUserDictView : public CAknView
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aModel
        */
        static CUserDictView* NewLC(CUserDictModel& aModel);

        /**
        * Destructor.
        */
        virtual ~CUserDictView();

    public: // New functions

        /**
        * Saves the changes of the element and updates ListBox.
        * It calls the model's function to add or to modify the entry to
        * the User dictionary.
        * @param aNewItem is the element after changes.
        * @param aOldItem is the element before changes.
        */
        void SaveL(const TUDictElement& aNewItem,
                    const TUDictElement& aOldItem) const;

        /**
        * Displays an information note with a message defined by aResourceId.
        * @param aResourceId is the ID of the resource of the message string.
        */
        void ShowInformationNoteL(TInt aResourceId) const;

        /**
        * From CAknView
        * Handles the client rectangle is changed.
        */
        void HandleClientRectChange();

    private: // Constructors

        /**
        * C++ default constructor.
        * @param aModel
        */
        CUserDictView(CUserDictModel& aModel);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // New functions

        /**
        * Shows a delete confirmation query.
        * @param aCount is the number of the deleting words.
        * @return zero, if user cancels confirmation.
        */
        TInt ShowDeleteConfQueryL(TInt aCount = 1) const;

        /**
        * Shows the form of User dictionary editor.
        * @param aItem is the reference to the entry item.
        */
        void ShowEditorFormL(const TUDictElement& aItem);

        /**
        * Re-initializes the registered word list and updates the ListBox.
        */
        void UpdateListL();

        /**
        * The iIdle's callback function
        * @param aView
        * @return zero
        */
        static TInt RegisterByAnotherApp(TAny* aView);

        /**
        * This function is only called when the application is launched
        * by another application.
        * The function starts the User dictionary editor and closes
        * this application after the editor is closed.
        */
        void DoRegisterByAnotherApp();

        /**
        * In this function the correct softkey is shown.
        */
        void ShowSoftkeyL();

    private: // Functions from base classes

        /**
        * From CAknView, returns this view Id.
        * @return this view Id
        */
        TUid Id() const;

        /**
        * From CAknView
        * Handles the commands.
        * @param aCommand
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CAknView, activates the view.
        * It creates and initializes CUserDictContainer object.
        * If it is started by another application, this function creates
        * the CIdle object to call RegisterByAnotherAppL().
        * @param aPrevViewId
        * @param aCustomMessageId
        * @param aCustomMessage
        */
        void DoActivateL(
                const TVwsViewId& aPrevViewId,
                TUid aCustomMessageId,
                const TDesC8& aCustomMessage);

        /**
        * From CAknView
        * Deactivates the view.
        */
        void DoDeactivate();

        /**
        * From CAknView
        */
        void HandleForegroundEventL(TBool aForeground);

        /**
        * From MEikMenuObserver
        * Initializes the menu pane.
        * @param aResourceId is the Resource ID of the menu pane
        * @param aMenuPane
        */
        void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

    private:    // Data

        // View Container Control
        CUserDictContainer* iContainer; // own

        // Japanese User Dictionary Model
        // It provides the access to the User dictionary data.
        CUserDictModel& iModel;

        // CIdle active object
        CIdle* iIdle; // own

        // Flag to forbid the re-initializing of the list.
        TBool iLockListUpdate;
        // ETrue is embedded
        TBool iEmbedded;
    };

#endif      // UDICTVIEW_H

// End of File
