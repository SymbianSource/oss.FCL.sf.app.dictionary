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
* Description:     Japanese user dictionary editor form.
*
*/






#ifndef UDICTFRM_H
#define UDICTFRM_H

//  INCLUDES
#include <AknForm.h>

// FORWARD DECLARATIONS
class CUserDictView;
class TUDictElement;

// CLASS DECLARATION

/**
*  This class defines the form of Japanese user dictionary editor.
*  Edits an entry of Japanese user dictionary, which has a word and
*  the reading.
*  @since 2.6
*/
class CUserDictForm : public CAknForm
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aItem is a reference to the entry item
        * @param aView
        */
        static CUserDictForm* NewL(const TUDictElement& aItem,
                                   CUserDictView& aView);

    private:

        /**
        * Destructor.
        */
        ~CUserDictForm();

    private:

        /**
        * C++ default constructor.
        * @param aItem is a reference to the entry item
        * @param aView
        */
        CUserDictForm(const TUDictElement& aItem, CUserDictView& aView);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // New functions

        /**
        * Gets the edited data, and sets the data to aEntry.
        * @param aEntry is set the edited data.
        */
        void GetData(TUDictElement& aEntry) const;

        /**
        * Sets the entry data to the controls which the form has.
        */
        void SetDataL() const;

        /**
        * Saves the entry, and if an error has occurred at the saving time,
        * it shows an error note.
        * @return EFalse, if it needs to fix the entry.
        */
        TBool SaveL();

        /**
        * Tests if the entry fields are blank.
        * @return EFalse, if the word and the reading are not empty.
        */
        TBool IsBlank() const;

        /**
        * Tests if the characters of the reading entry is valid.
        * @return EFalse, the reading entry has invalid characters.
        */
        TBool IsValidReadingL(const TUDictElement& aItem) const;

    private: // Functions from base classes

        /**
        * From CEikDialog
        * Initializes the form.
        */
        void PreLayoutDynInitL();

        /**
        * From CEikDialog, handles exit event of the form.
        * It calls SaveL(). If the saving finishes with EFalse,
        * this function returns EFalse.
        * @param aKeycode
        * @return ETrue, when exits the form.
        */
        TBool OkToExitL(TInt aKeycode);

        /**
        * From MCoeControlObserver, handles event of the form state changes.
        * It switches the "Done" softkey visible or hidden.
        * @param aControlId
        */
        void HandleControlStateChangeL(TInt aControlId);

        /**
        * From CCoeControl
        * Handles key events.
        * @param aKeyEvent
        * @param aType
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
                                    TEventCode aType);

    private:    // Data

        // The Entry keeping the initial state
        const TUDictElement& iInitEntry;

        // The reference of View
        // It's used for calling the saving function.
        CUserDictView& iView;

        // The resource id for displaying softkey
        TUint iSoftkeyResource;
    };

#endif      //  UDICTFRM_H

// End of File
