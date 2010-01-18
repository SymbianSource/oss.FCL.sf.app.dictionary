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
* Description:     Japanese user dictionary Application UI class.
*
*/






#ifndef UDICTAPPUI_H
#define UDICTAPPUI_H

//  INCLUDES
#include <aknViewAppUi.h>

// CLASS DECLARATION

/**
*  Japanese user dictionary Application UI class.
*  @since 2.6
*/
class CUserDictAppUi : public CAknViewAppUi
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CUserDictAppUi();

        /**
        * Destructor.
        */
        virtual ~CUserDictAppUi();

    private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
 
    private:   // Functions from base classes

        /**
        * From CEikAppUi
        * Handles the commands.
        * @param aCommand to be handled
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CEikAppUi
        * Handles the commands.
        */
        void HandleScreenDeviceChangedL();
    };

#endif      // UDICTAPPUI_H
            
// End of File
