Changes 07/28/26
* Changes for Tomcat 11
* Changes for buildlplr

***** There are a lot of changes missing from this readme.md. I should look over the 10c file and add to here. I just added the latest from 2026.
Changes 06/25/2026
* Changed the generation code to use text-start/text-end instead of text-right/text-left for bootstrap5.

Changes 06/16/2026
* Added alternate jsp generation for bootstrap5. This requires a zeidon.ini insertion line of the following.
;; at the GMC we are moving to bootstrap 5. This needs some different jsp generation.
Bootstrap5=Y

Changes 06/02/26
* Changes for jasperreports 7.0.6 version. This affects jasper xls exports and is changes to jsp pages, only when an export action is present.

Change 05/20/26
* jsp generation changes for TinyMCE (changed location of tinymce.min.js and added TinyMCESet.js)
* js generation changes for TinyMCE (add setMLEdits() to actions on page that have tinymce)

Change 04/27/26
* Move js call processControlActions from the bottom of the jsp file to "_AfterPageLoaded" code in the js file. This
  is because the new TinyMCE textarea is not disabled correctly unless it is in afterpageloaded (a change was also made to zeidonctrlactions.js in lplr).
* Changed jsp include location of tinymce js code from 
         "./js/tinymce/jscripts/tiny_mce/tiny_mce.js"
  to
         "./js/tinymce/tinymce.min.js"
  This can be changed with a value in zeidon tools zeidon.ini file if necessary (TinyMCEJavaScript=)
* JSP change MLEdit log line from info to debug
  task.log( ).info( "Entity.Attribute: " + strErrorMapValue ) to
  task.log( ).debug( "Entity.Attribute: " + strErrorMapValue ) 
* For MLEdit controls with wysiwyg class. We had been not including meSimple if another class...



Changes made 04/25/24
* Incorporated Don's autodesign changes
* Added a new field to TZCMLPLO.LOD (LPLR.AutodesignBase) 
* Added AutodesignBase field to TZCMLPLD.PWD (Workstations "Application Maintenance" window)
* Some fixes to zdr cpp files due to copy/paste crash errors.

# Zeidon 10d 

## Compiling

### Requires
* Visual Studio 2017 Community Edition

### Environment
Run vcvarsall.bat to set VS environment (Look in "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat")
Set following environment variables
kzd = install drive.  Defaults to 'c'.
kzv = Zeidon tools version.  Defaults to '10d'.

### Compile
Run MAKEALL.BAT.

## Compiling issues
### 'Access is denied' on an executable like qspawn.exe
Check the permissions on the executable.  It looks like git might not set the permissions correctly on checkout.  Your local user must have read + execute authority

Changes 10/13/22

* Some bootstrap menu/banner changes.
* Change in tzwdlg6o.vml // KJS 11/04/22 - If an action changes the language, we want to call getLanguageObject to make sure we are using that.

Changes 08/12/22

* Disable in Common was not working for MLEdit and for "Select Entity" combo box. This has been fixed.

Changes 07/26/22
* Migration fix - copy over all source files (was not looping through all the source files for a dialog/object.
* Changed some tools naming to "Information Centric". Not sure this is permanent...
* JSP Generation - 
  took out Calendar size in grid
  MLEdit in grid

Change made 05/25/22

* Merged in DonC changes for LOD attribute derived attribute rule code. This is not complete, but mainly for viewing
  at the moment.
* KJS changes for Workstation Migration. More duplication elimination. Also allows user to start with moving a LOD like
  sHost.LOD and it will create the ER/PDG/LOD etc.


Change made 04/29/22

* DonC made some changes in the ER "Compare/Merge ERD" functionality for the purposes of demo.
  A button was added "Compare Entities" - which shows the more high level, entity merge.
  This would be for the "component merge" functionality, where a whole other lplr is merged into
  the existing project. This is not fully funtional. The original compare is the button "Compare Attributes"
* In another LPLR migration, there are errors that need to be looked into that cause duplicate domain names to be created.
  Because of this a button was added to domain list "Delete Duplicate" that loops through the domain list. When a duplicate domain
  is located, we check if it is in the ER, if not, we delete it. Also, we previously could not delete duplicate domains because the CommitOI would not
  allow a save if a duplicate name was in existence. This is taken off to allow domain clean up but we check against the ER and do not allow a domain
  to be deleted if it is used on an attribute in the ER.
* There was an error in jsp generation code having to do with bootstrap and banners, where old code was lost. That has been fixed.
* Also with jsp generation, for variable headers if the variable value was null, a <th> wasn't being created but a <td> was. Now, both are created.


Changes made to 10d (update on 04/12/2022):

* Changes from Don for ER merge functionality (merging Entities). This is for demo purposes, overall not yet complete.
* JSP generation - for bootstrap, if there is no side menu, then have side menu default to close.
* Made Operation list window larger.
* Small bug fix in tzwebgl3.vml having to do with adding a menu where the action is tied to another control on the window.

Changes made to 10d (update on 12/3/2021):

* Allow operation calls between vml in same pwd/lod if there are more than one source file.
* try/catch around <form data in jsp. This is hopefully to help with the issue of when an error occurs on a control (like a domain value missing) and the page does not completely render.
  Looking at tomcat console or log file should indicate what the error is.
* In the Dialog tool, there are times when mapping a control and you can not sort the attribute list... now all mapping areas should allow sort or attribiute name.
* Global Operation - if the source file of the operation is java, if user clicks "Edit Operation", give a pop up message that it can't be updated here, and don't create .java file in lplr.
* In jsp, menu <li for side/top menu use Menu tag for the name/id not the Action tag (which it had been using)
* Give a pop up message when generating to bootstrap jsp, and the most outer div does not contain a class of "card card-body" and we do not have the following zeidon.ini setting: 
  BootstrapOuterCard=Y (which will automatically put a "div" with the class "card card-body mb-2").
  Without the "card card-body" class, the page does not always render properly and in the case of file upload/transfer, the upload fields do not display at all.

  Here is a comment in code (in case it is helpful for understanding). 
         // If we are generating to bootstrap with a zeidon.ini setting of BootstrapOuterCard=Y, and we are on the most outer group box,
         // we are going to surround that <div> with a <div class='card card-body mb-2'. This is so that the developer does not have
         // to create so many divs within divs on the page.

