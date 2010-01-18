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
* Description:     This class represents an element of Japanese user dictionary
*                for Japanese FEP.
*
*/






#ifndef UDICTELEMENT_H
#define UDICTELEMENT_H

//  INCLUDES
#include    <e32base.h>
#include    <PtiUserDicEntry.h>
#include    "UserDict.hrh"

// CLASS DECLARATION

/**
* This class represents an element of Japanese user dictionary
* for Japanese FEP.
* It contains two string buffers for a word text and  a reading text.
*  @since 2.6
*/
class TUDictElement  : public MPtiUserDictionaryEntry
    {
    public:

       /**
        * C++ default constructor.
        */   
        TUDictElement();

    public: // New functions

        /**
        * Returns the descriptor which has a ListBox's separator character,
        * for using to ListBox item.
        * @return the descriptor with a ListBox separator.
        */
        inline const TDesC& ListItemText() const;

        /**
        * Sets aReading to the buffer of the reading descriptor.
        * @param aReading is set to the reading buffer. 
        */
        inline void SetReading(TDesC& aReading);

        /**
        * Compares to the other element.
        * @param aElement to be compared to this 
        * @return zero, if the both elements are identical.
        */
        inline TInt operator!=(const TUDictElement& aElement) const;
        
        /**
        * Tests if this element is empty.
        * @return zero, if this element isn't empty.
        */
        inline TInt IsEmpty() const;

    public: // Functions from base classes

        /**
        * From MPtiUserDictionaryEntry 
        * Returns the word descriptor.
        * @return the word descriptor.
        */
        TPtrC Word() const;

        /**
        * From MPtiUserDictionaryEntry 
        * Returns the reading descriptor.
        * @return the reading descriptor.
        */
        TPtrC Reading() const;

        /**
        * From MPtiUserDictionaryEntry 
        * Sets aWord to the buffer of the word descriptor.
        * @param aWord is set to the word buffer. 
        */
        void SetWord(TDesC& aWord);

    private: // Functions from base classes

        /**
        * From MPtiUserDictionaryEntry 
        * Sets word and reading descriptor.
        *
        * @param aWord is set to the word buffer.
        * @param aReading is set to the reading buffer.
        */
        void SetWord(TDesC& aWord, TDesC& aReading);

    private: // Data

        // Word buffer
        // The buffer needs one additional room for a ListBox separator. 
        TBuf<EUseDictWordBuffer + 1> iWord;
 
        // Reading buffer
        TBuf<EUseDictReadingBuffer> iReading;
    };

#include    "UdictElement.inl"

#endif      // UDICTELEMENT_H
            
// End of File
