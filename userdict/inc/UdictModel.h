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
* Description:     The engine class of Japanese user dictionary.
*                It wraps PtiEngine to access the User dictionary.
*
*/






#ifndef UDICTMODEL_H
#define UDICTMODEL_H

//  INCLUDES
#include    "UdictElement.h"
#include    <badesca.h>

// DATA TYPES
typedef CArrayFixFlat<TUDictElement> CUserDictList;

// FORWARD DECLARATIONS
class CPtiEngine;
class MPtiUserDictionary;

// CLASS DECLARATION

/**
*  The engine of Japanese user dictionary.
*  The main role of this class is accessing the User dictionary data via
*  CPtiEngeine.
*  @since 2.6
*/
class CUserDictModel : public CBase, public MDesCArray
    {   
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CUserDictModel* NewL();

        /**
        * Destructor.
        */
        ~CUserDictModel();

    public: // New functions
        
        /**
        * Returns the self object as interface to access the registered
        * word list.
        * @return a pointer of the interface of the registered word list.
        */
        inline MDesCArray* List();
        
        /**
        * Gets an element of the registered word list.
        * @param aIndex is the position of the element.
        * @return a reference to an element of the registered word list,
        *         located at aIndex.
        */
        inline const TUDictElement& ListItem(TInt aIndex) const;

        /**
        * Tests if the User dictionary data is changed by another application.
        * @return ETrue, if the User dictionary data is changed.
        */
        inline TBool IsChangedByAnotherApp() const;
        
        /**
        * Sets the flag which notifies the User dictionary is changed by
        * another application.
        * @param aFlag is set to the of User dictionary update flag. 
        */
        inline void SetChangeNotice(TBool aFlag);

        /**
        * Tests if the user dictionary has enough space to add word or not.
        * @return ETrue, the user dictionary has no space to register.
        */
        inline TBool IsDictionaryFull();

        /**
        * Reload User dictionary data to reflect the changes by another
        * application.
        */
        inline void ReloadDictInfoL();
        
        /**
        * Stops the file storing in registering and deleting until SaveNow()
        * is called.
        */
        inline void DelaySavingL();

        /**
        * Stores cahges of the dictionary data to the file.
        */
        inline void SaveNowL();

        /**
        * Adds a new word to the User dictionary.
        * This function might leave. It leaves with KErrAlreadyExists if the 
        * word is already registered, KErrDiskFull if the dictionary has no
        * space to register.
        *
        * @param aItem is added to the User dictionary
        * @return index of a new word inserted to the registered word list. 
        */
        TInt AddNewWordL(const TUDictElement& aItem);

        /**
        * Deletes the word from the User dictionary.
        * @param aIndex is the position of the deleted item in the word list.
        */
        void DeleteWordL(TInt aIndex);
        
        /**
        * Modifies the registered word of User dictionary.
        * If modifying the word is failed, it leaves.
        *
        * @param aIndex is the position of the modified word
        * @param aItem is a modified entry 
        * @return index of a modified word relocated in the registered
        *         word list.
        */
        TInt ModifyWordL(TInt aIndex, const TUDictElement& aItem);

        /**
        * Initializes the registered word list.
        * It copies the User dictionary data to iItemList, which is the 
        * registered word list and sorts the list by Unicode collation
        * algorithm.
        */
        void InitListL();

    private:

        /**
        * C++ default constructor.
        */
        CUserDictModel();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private:  // Functions from base classes
                
        /**
        * From MDesCArray, returns a number of elements in the array.
        * @return a number of elements in the registered word list. 
        */
        TInt MdcaCount() const;
        
        /**
        * From MDesCArray,
        * Retruns a descriptor for the ListBox's item.
        * @param aIndex is a position of the registered word list.
        * @return a descriptor for the ListBox's item representing the element
        *         located at aIndex within the word list.
        */
        TPtrC16 MdcaPoint(TInt aIndex) const;

    private:    // Data

        // The array of registered words.
        // The original User dictionary data is copied to this array.
        CUserDictList* iItemList; // own

        // The predictive text input engine.
        // It provides the API to access the User dictionary data.
        CPtiEngine* iPtiEngine; // own

        // PtiEngine's user dictionary interface.
        MPtiUserDictionary* iDictionary ;

    private:
    
        class TWordListKey : public TKeyArrayFix
            {
            public:  // Constructor

                /**
                * C++ default constructor.
                */
                TWordListKey();

            private:  // Functions from base classes

                /**
                * From TKey, Compares the keys of two elements.
                * @param aLeft is the index of the Left element
                * @param aRight is the index of the Right element
                * @return zero, if both elements are identical.
                *         positive, if the left element is greater.
                *         negative, if the right element is greater.
                */            
                TInt Compare(TInt aLeft, TInt aRight) const;
            };
    };

#endif      // UDICTMODEL_H
            
// End of File
