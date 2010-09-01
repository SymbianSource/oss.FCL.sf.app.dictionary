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
* Description:     Japanese user dictionary Document class.
*
*/






#ifndef UDICTDOCUMENT_H
#define UDICTDOCUMENT_H

//  INCLUDES
#include <AknDoc.h>

// FORWARD DECLARATIONS
class CEikAppUi;
class CUserDictModel;

// CLASS DECLARATION

/**
*  Japanese user dictionary Document class.
*  It owns CUserDictModel object.
*  @since 2.6
*/
class CUserDictDocument : public CAknDocument
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aApp is reference of the App.
        */
        static CUserDictDocument* NewL(CEikApplication& aApp);
        
        /**
        * Destructor.
        */
        virtual ~CUserDictDocument();

        /**
        * Returns CUserDictModel object.
        * @return CUserDictModel object.
        */
        CUserDictModel& Model() const;

    private:

        /**
        * C++ default constructor.
        * @param aApp is reference of the App.
        */
        CUserDictDocument(CEikApplication& aApp);
    
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:  // Functions from base classes

        /**
        * From CEikDocument.
        * Create user dictionary's AppUI object.
        * @return the created CEikAppUi object.
        */
        CEikAppUi* CreateAppUiL();

    private:  // Data

        // Japanese user dictionary model object
        CUserDictModel* iModel; // own

    };

#endif      // UDICTDOCUMENT_H
            
// End of File
