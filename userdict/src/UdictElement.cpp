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





// INCLUDE FILES
#include    "UdictElement.h"

// CONSTANTS
_LIT(KListSeparator, "\t");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TUDictElement::TUDictElement
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TUDictElement::TUDictElement()
:iWord(KListSeparator), iReading(KNullDesC)
    {
    }

// -----------------------------------------------------------------------------
// TUDictElement::Word
// -----------------------------------------------------------------------------
//
TPtrC TUDictElement::Word() const 
    {
    // Omits the head ListBox's separator character in the word buffer.
    return iWord.Mid(1);
    }

// -----------------------------------------------------------------------------
// TUDictElement::Reading
// -----------------------------------------------------------------------------
//
TPtrC  TUDictElement::Reading() const
    {
    return iReading;
    }

// -----------------------------------------------------------------------------
// TUDictElement::SetWord
// -----------------------------------------------------------------------------
//
void TUDictElement::SetWord(TDesC& aWord) 
    {
    // Sets a ListBox's item separator in head.
    iWord = KListSeparator;
    iWord.Append(aWord);
    }

// -----------------------------------------------------------------------------
// TUDictElement::SetWord
// -----------------------------------------------------------------------------
//
void TUDictElement::SetWord(TDesC& aWord, TDesC& aReading)
    {
    SetWord(aWord);
    SetReading(aReading);
    }

//  End of File  
