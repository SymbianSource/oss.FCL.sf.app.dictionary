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
* Description:     Inline functions of Japanese user dictionary's element class.
*
*/






// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TUDictElement::ListItemText
// Returns the descriptor with a ListBox's separator character.
// -----------------------------------------------------------------------------
//
inline const TDesC&  TUDictElement::ListItemText() const 
    {
    return iWord;
    }

// -----------------------------------------------------------------------------
// TUDictElement::SetReading
// -----------------------------------------------------------------------------
//
inline void TUDictElement::SetReading(TDesC& aReading) 
    {
    iReading = aReading;
    }

// -----------------------------------------------------------------------------
// TUDictElement::operator!=
// -----------------------------------------------------------------------------
//
inline TInt TUDictElement::operator!=(const TUDictElement& aElement) const
    {
    return !((Word() == aElement.Word()) && (Reading() == aElement.Reading()));
    }

// -----------------------------------------------------------------------------
// TUDictElement::IsEmpty
// -----------------------------------------------------------------------------
//
inline TInt TUDictElement::IsEmpty() const
    {    
    return (Word().Length() == 0 || Reading().Length() == 0);
    }

//  End of File  
