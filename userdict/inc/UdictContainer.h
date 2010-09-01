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
* Description:     Japanese user dictionary View container control. It contains
*                a single style list box to show the registered words.
*
*/






#ifndef UDICTCONTAINER_H
#define UDICTCONTAINER_H

//  INCLUDES
#include <coecntrl.h>
#include <AknWaitNoteWrapper.h> 

// FORWARD DECLARATIONS
class CAknSingleStyleListBox;
class CEikListBox;
class CGulIcon;
class CAknView;
class CUserDictModel;

// CLASS DECLARATION
/**
*  Japanese user dictionary Container class.
*  It contains a single style list box to show the registered words.
*  CUserDictView controls this class.
*  @since 2.6
*/
class CUserDictContainer : public CCoeControl, public MAknBackgroundProcess
    {
    public:  // Constructors and destructor
      
        /**
        * C++ default constructor.
        * @param aModel
        * @param aView
        */
        CUserDictContainer(CUserDictModel& aModel, CAknView& aView);

        /**
        * 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CUserDictContainer();

    public: // New functions

        /**
        * It sets the focus to the item which is located at aIndex and redraws 
        * the Listbox.
        * @param aIndex is set to the Listbox's current item. 
        */
        void SetFocusL(TInt aIndex) const;

        /**
        * Deletes the selected items in the Listbox.
        * If the Listbox has marked items, it deletes the all marked items.
        * If it doesn't have any marked item, it deletes the focused item.  
        */
        void DeleteItemL();

        /**
        * Gets the Listbox object.
        * @return the Listbox object.
        */
        inline CEikListBox* ListBox() const;
 
    private:  // New functions

        /**
        * Sets a marking icon array to the Listbox.
        */
        void SetGraphicIconL();

        /**
        * It shows a Wait Note during deleting the marked items.
        */
        void ShowWaitNoteL();

        /**
        * Creates the icon and adds it to the array if it was successful
        *  @since 2.8
        */
        void CUserDictContainer::CreateIconAndAddToArrayL(
                                    CArrayPtr<CGulIcon>*& aIconsArray,
                                    const TDesC& aIconFile,
                                    TInt aBitmap, TInt aMask = -1);


    private:  // Functions from base classes

        /**
        * From CCoeControl
        * Handles size changed.
        */
        void SizeChanged();

        /**
        * From CCoeControl
        * Gets a number of the component controls.
        * @return a number of the component control.
        */
        TInt CountComponentControls() const;

        /**
        * From CCoeControl
        * Gets the component control.
        * @param aIndex is index of the component control
        * @return the component control
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

        /**
        * From CCoeControl
        * Handles key events.
        * @param aKeyEvent
        * @param aType
        * @return EKeyWasConsumed, when it handles the event. 
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
                                    TEventCode aType);
          
        /**
        * From CCoeContro, gets help context.
        * @param aContext
        */ 
        void GetHelpContext(TCoeHelpContext& aContext) const;
        
        /**
        * From MAknBackgroundProcess,
        * It has the actual process in the wait note's loop.
        * It deletes the selected words by descending order of list index.
        * This function is called repeatedly until all selected words are
        * deleted.
        */
        void StepL();
        
        /**
        * From MAknBackgroundProcess, tests whether Wait Note process is done.
        * When iWaitNoteCounter is under zero, it finishes the process.
        * @return ETrue, when the process is done.
        */
        TBool IsProcessDone() const;

    private:    // Data 

        // The ListBox showing registered words 
        CAknSingleStyleListBox* iListBox; // own
        
        // Japanese user dictionary Model
        CUserDictModel& iModel;

        // View, the controller of this class
        CAknView& iView;

        // The counter for the Wait Note process.
        // It's used for count of deleting words
        TInt iWaitNoteCounter;
      
    };

#endif      // UDICTCONTAINER_H   
            
// End of File
