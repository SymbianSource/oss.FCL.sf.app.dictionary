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
* Description:     Japanese user dictionary Application class.
*
*/






#ifndef UDICTAPP_H
#define UDICTAPP_H

//  INCLUDES
#include <aknapp.h>

// CONSTANTS
// UID of the application
const TUid KUidUserDict = { 0x101F8645 };

// CLASS DECLARATION

/**
*  Japanese user dictionary Application class.
*  @since 2.6
*/
class CUserDictApp : public CAknApplication
    {
    private: // Functions from base classes

        /**
        * From CApaApplication.
        * Returns application's UID.
        * @return the value of KUidUserDict.
        */
        TUid AppDllUid() const;

        /**
        * From CApaApplication.
        * Creates CUserDictDocument document object.
        * @return the created document object.
        */
        CApaDocument* CreateDocumentL();
    };

#endif      // UDICTAPP_H   
            
// End of File
