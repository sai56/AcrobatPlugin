/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1998-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 -------------------------------------------------------------------*/
/** 
\file BasicPlugin.cpp

  - This file implements the functionality of the BasicPlugin.
*********************************************************************/


// Acrobat Headers.
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#include <windows.h>
#include <shellapi.h>
#endif
#include "FileUploader.h"
/*-------------------------------------------------------
	Constants/Declarations
-------------------------------------------------------*/
// This plug-in's name, you should specify your own unique name here.
//#pragma  message ("Please specify your own UNIQUE plug-in name. Remove this message if you have already done so")

const char* MyPluginExtensionName = "ToolbuttonPlugin";


char* concatenate(char* first, char* second) {
	char* res = (char*)malloc(1 + strlen(first) + strlen(second));
	strcpy(res, first);
	strcat(res, second);
	return res;
}

/* A convenient function to add a menu item for your plugin.

ACCB1 ASBool ACCB2 PluginMenuItem(char* MyMenuItemTitle, char* MyMenuItemName);
*/

/*-------------------------------------------------------
	Functions
r-------------------------------------------------------*/

/* MyPluginSetmenu
** ------------------------------------------------------
**
** Function to set up menu for the plugin.
** It calls a convenient function PluginMenuItem.
** Return true if successful, false if failed.
*/

/*
ACCB1 ASBool ACCB2 MyPluginSetmenu()
{
	// Add a new menu item under Acrobat SDK submenu.
	// The new menu item name is "ADBE:BasicPluginMenu", title is "Basic Plugin".
	// Of course, you can change it to your own.
	return PluginMenuItem("Basic Plugin", "ADBE:BasicPluginMenu"); 
}
*/	

/**		BasicPlugin project is an Acrobat plugin sample with the minimum code 
	to provide an environment for plugin developers to get started quickly.
	It can help Acrobat APIs' code testing, too.  
		This file implements the functionality of the BasicPlugin. It adds a 
	new menu item that will show a message of some simple information about 
	the plugin and front PDF document. Users can modify and add code in this 
	file only to make a simple plugin of their own.   
		
		  MyPluginCommand is the function to be called when executing a menu.
	This is the entry point for user's code, just add your code inside.

	@see ASExtensionGetRegisteredName
	@see AVAppGetActiveDoc
	@see PDDocGetNumPages
*/ 



//Define a toolbar button callback function

ACCB1 void ACCB2 LoadGooglePage(void* data){

	/*
		When the button gets pressed,
		a confirmation popup shows up
		whether to visit the specified site.
	*/
	ASBool val =  AVAlertConfirm("Do you want to visit 'google.com' ?");
	

	/*
		If the user presses "OK"
		then perform the necessary actions 
		(In this case launches google.com and makes a POST request to a dummy website)
	*/
	if (val == true) {


		// Get access to the active document for adding tags 
		AVDoc avDoc = AVAppGetActiveDoc();
		PDDoc currentPDDoc = AVDocGetPDDoc(avDoc);
		ASFile currentFile = PDDocGetFile(currentPDDoc);
		ASPathName currentFilePath = ASFileAcquirePathName(currentFile);
		char* filePath = ASFileSysDisplayStringFromPath(NULL,currentFilePath);

		//AVAlertNote(filePath);
		

		// Declare a FileUploader object
		FileUploader fileUploader;


		/* 
		   Call the uploadFile function to make a POST request
		   Storing the result of the output in response for debugging purposes
		*/
		const char* response = fileUploader.uploadFile("http://localhost:5000/",filePath);

		/*
			Displays an alert with message :
			RESPONSE OR ERROR
		*/
		AVAlertNote(response);



		// Launching the specified web-page
		ShellExecuteA(NULL, "open", "https://google.com", NULL, NULL, SW_SHOWNORMAL);
		
		

		
		char* clientVal = "sample_url";
		char* idVal = "206142";
		char* stateVal = "CERTIFIED";
		char* lVal = "AB";


		ASText orig = ASTextNew();
		ASText infoKey = ASTextFromPDText("Keywords");


		PDDocGetInfoASText(currentPDDoc, infoKey, orig);
		
		
		char* text = "<X><Client>";
		char* additional = concatenate(text, clientVal);
		text = "</Client><Id>";
		additional = concatenate(additional, text);
		additional = concatenate(additional, idVal);
		text = "</Id><State>";
		additional = concatenate(additional, text);
		additional = concatenate(additional, stateVal);
		text = "</State><l>";
		additional = concatenate(additional, text);
		additional = concatenate(additional, lVal);
		text = "</l></X>";
		additional = concatenate(additional, text); 

		//char* val = concatenate(original, additional);
		//ASText addn = ASTextFromPDText(additional);
		
		char* original = ASTextGetEncodedCopy(orig, ASTextGetBestEncoding(orig, (ASHostEncoding)PDGetHostEncoding()));
		if(strlen(original)!=0)
			original = concatenate(original, "\n");
		
		char* value = concatenate(original, additional);
		int t = strlen(value);
		char p[64];
		sprintf(p, "%d", t);
		
		//AVAlertNote(p);
		//ASText val = ASTextFromPDText(value);
		//ASTextCat(orig, addn);
		
		PDDocSetInfo(currentPDDoc, "Keywords", value,t);
		PDDocSave(currentPDDoc, PDSaveFull | PDSaveLinearized, currentFilePath, ASGetDefaultFileSys(), NULL,NULL);
		
		/*
			// Additional metadata added to the properties of the document 
			ASText namespaceName = ASTextFromPDText("http://ns.adobe.com/Custom");
			ASText namespacePrefix = ASTextFromPDText("Custom");
			ASText path = ASTextFromPDText("test1");
			ASText newValue = ASTextFromPDText("2");
			PDDocSetXAPMetadataProperty(myPDDoc,namespaceName,namespacePrefix,path,newValue);
		*/
	}
		
}



/**
ACCB1 void ACCB2 MyPluginCommand(void *clientData)
{
	// get this plugin's name for display
	ASAtom NameAtom = ASExtensionGetRegisteredName (gExtensionID);
	const char * name = ASAtomGetString(NameAtom);
	char str[256];
	sprintf(str,"This menu item is added by plugin %s.\n", name);

	// try to get front PDF document 
	AVDoc avDoc = AVAppGetActiveDoc();

	if(avDoc==NULL) {
		// if no doc is loaded, make a message.
		strcat(str,"There is no PDF document loaded in Acrobat.");
	}
	else {
		// if a PDF is open, get its number of pages
		PDDoc pdDoc = AVDocGetPDDoc (avDoc);
		int numPages = PDDocGetNumPages (pdDoc);
		sprintf(str,"%sThe active PDF document has %d pages.", str, numPages);
	}

	// display message
	AVAlertNote(str);

	return;
}
*/

/* MyPluginIsEnabled
** ------------------------------------------------------
** Function to control if a menu item should be enabled
.
	if (val == true)
		AVSysBeep(0);

** Return true to enable it, false not to enable it.
*/

/*
ACCB1 ASBool ACCB2 MyPluginIsEnabled(void *clientData)
{
	// always enabled.
	return true;
	
	// this code make it is enabled only if there is a open PDF document. 
	 return (AVAppGetActiveDoc() != NULL); 
}
*/


