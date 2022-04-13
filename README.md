# Zeidon 10d 

## Compiling

### Requires
* Visual Studio 2015 Community Edition

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

