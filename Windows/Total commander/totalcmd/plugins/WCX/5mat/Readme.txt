MATLAB® MAT-file Viewer 1.6.11.0 for Total Commander
====================================================

 * License:
-----------

This software is released as freeware.


 * Disclaimer:
--------------

This software is provided "AS IS" without any warranty, either expressed or
implied, including, but not limited to, the implied warranties of
merchantability and fitness for a particular purpose. The author will not be
liable for any special, incidental, consequential or indirect damages due to
loss of data or any other reason.


 * Installation:
----------------

 1. The MATLAB MAT-file Viewer uses exported functions from external MATLAB
    libraries of The MathWorks. Depending on your MATLAB installation you
    have to decide if you need to download the MATLAB libraries:
    - Everything is fine if you have installed MATLAB >= R2006b.
    - Everything is fine if you have installed MATLAB >= R14 and you need not
      to use MAT-files in HDF5-based format.
    - Everything is fine if you have installed MATLAB >= R13 and you need not
      to use compressed, Unicode encoded or HDF5-based MAT-files.
    - You can download and install the appropriate version of the MATLAB
      Compiler Runtime (MCR) starting from MATLAB R2012a from
        - http://www.mathworks.com/products/compiler/mcr
    - If neither MATLAB nor the MATLAB Compiler Runtime (MCR) is installed or if
      you want to change the version of the MATLAB libraries you can download
      either the MATLAB R13SP2 libraries or the MATLAB R2013b (x86/x64) libraries
      by means of the installer 5mat_setup.exe. It is recommended to extract the
      downloaded library archive to the plugin directory. You can put the 64-bit
      libraries into the x64 subdirectory of the plugin directory if you already
      have the 32-bit libraries in the plugin directory.
 2. The latest MATLAB R2013b libraries are linked to the Microsoft Visual C++
    2010 SP1 Runtime Libraries. If required download and install the appropriate
    redistributable packages.
      Microsoft Visual C++ 2010 SP1 Redistributable Package (x86)
      - http://www.microsoft.com/download/details.aspx?id=8328
      Microsoft Visual C++ 2010 SP1 Redistributable Package (x64)
      - http://www.microsoft.com/download/en/details.aspx?id=13523
 3. Registration as content, lister and packer plugin in Total Commander and
    file association of .fig and .mat with stand-alone MATLAB MAT-file Viewer
    can be automatically set up by the installer 5mat_setup.exe.


 * Installation Remarks:
------------------------

 o If you have MATLAB installed you will find the MATLAB 32-bit libraries in
   directory %MATLAB_ROOT%\bin\win32\ and the MATLAB 64-bit libraries in
   directory %MATLAB_ROOT%\bin\win64\
 o The MATLAB libraries are first searched in the plugin directory and for 64-bit
   Total Commander also in the x64\ subdirectory of the plugin directory.


 * Update Remarks:
------------------

 o The development of the DLL Diagnostic Tool 5mat_dll_diag.exe (formerly named
   DLL Check & Download Tool 5mat_dll_check.exe) is discontinued. This tool
   is no longer part of the plugin installer and will also be deleted when
   updating from MAT-file Viewer >= 1.3.1.0.
 o With MAT-file Viewer 1.6.4.0 the directory of 5mat.ini changed from the plugin
   directory to the same directory as the initialization file (wincmd.ini) of Total
   Commander. If Total Commander is not installed the directory %APPDATA%\GHISLER
   is used as the default directory for 5mat.ini.


 * Description:
---------------

The MATLAB MAT-file Viewer is three plugins at the same time.

 1. It adds packer support for the MAT-file type, i.e. you can treat MATLAB
    MAT-files (Version 4, 5 and 7) like archives. For examples, it is possible
    to view variables of a MAT-file, to compare the variables of two open
    MAT-files, to delete variables from a MAT-file, or to add real numeric data
    from a text file to a MAT-file.

    The settings of the [packer] section of the 5mat.ini are:
    - DoubleDigitsDisplay: number of displayed digits for double variables
    - SingleDigitsDisplay: number of displayed digits for single variables
    - ExpandSingleStructAsVars: If set to 1, expand the structure if the
      MAT-file contains just a single structure array.

    Remark: Like any other packer plugin 5mat.wcx can also be used with ArchView
    lister plugin >= 0.9.0.0 for Total Commander.

 2. It can be used as content plugin, too, showing information about
    - MAT-file version,
    - platform (Version 5 and 7 MAT-file only),
    - creation time (Version 5 and 7 MAT-file only),
    - number of variables.

 3. The lister view shows the variables of a MAT-file, their size (number of
    elements), their size in Bytes and their class. It is also possible to
    delete a variable by pressing DEL and to view a variable by pressing ENTER.
    In order to view variables you need to configure the path to the stand-alone
    Lister (lister.exe) in the 5mat.ini file ([lister] section). The stand-alone
    Lister can be downloaded from http://www.ghisler.com/lister or during
    installation process.

    Further settings of the [lister] section of the 5mat.ini are:
    - BackColor: background color
    - TextColor: text color
    - Column0Width: width of first column
    - Column1Width: width of second column
    - Column2Width: width of third column
    - Column3Width: width of fourth column
    - DeleteConfirmation: ask for a delete confirmation
    - StatusBar: display status bar
    - ColumnSort: column number to sort for
    - ColumnSortOrder: column sort order (0 = ascending, 1 = decending)
    - PrintFont: font for printing (default: Fixedsys)
    - PrintFontSize: font size for printing (default: 12)

    Last not least, a simple stand-alone MAT-file Viewer 5mat.exe has been
    added that uses the lister plugin interface of the MAT-file Viewer.
    - You can open and view a MAT-file in stand-alone MAT-file Viewer by
      launching "5mat.exe matfile.mat"
    - If you associated .fig / .mat with stand-alone MAT-file Viewer you can
      just double-click the MAT-file.


 * Examples:
------------

 o File example_v4.mat is a Version 4 MAT-file.
 o File example_v5.mat is a Version 5 MAT-file that MATLAB <= R7 can not load.
 o File example_v7.mat is a compressed (and Unicode encoded) Version 5 MAT-file
   that MATLAB <= R13SP2 can not load.
 o File example_v7.3.mat is a Version 7 MAT-file in HDF5-based format that
   MATLAB <= R2006a can not load.


 * Limitations:
---------------

 o Only real numeric data can be packed to a MAT-file or copied from one
   MAT-file to another MAT-file. Any numeric data is converted to a double
   vector with 16 digits of precision. Check the simple text file example.dat
   for how valid data has to look like.
 o The variable size (in Bytes) for mxCELL_CLASS and mxSTRUCT_CLASS is based on
   MATLAB >= R13. Previous versions of MATLAB (e.g. MATLAB R10) allocated
   more memory for handling cells and structures and therefore displayed
   increased size information.
 o Variable classes mxINT64_CLASS, mxUINT64_CLASS, mxFUNCTION_CLASS,
   mxOPAQUE_CLASS, mxOBJECT_CLASS and mxUNKNOWN_CLASS can not be unpacked.


 * Known problems:
------------------

 o When unpacking variables from MAT-files the progress bar of Windows Vista
   performs rather badly. As a workaround you may set the NewStyleProgress
   option of Total Commander to 0 in [Configuration] section of the
   initialization file of Total Commander.
 o The size (in Bytes) of sparse arrays is not correctly calculated.
 o The size (in Bytes) of cells and structures of MAT-files in HDF5-based
   format is not correctly calculated due to a bug of the MATLAB API functions.
 o Timeseries objects are not supported by the MATLAB API functions.


 * ChangeLog:
-------------

 o Version 1.6.11.0 (21.10.2013)
   - added changes to support MATLAB R2013b
   - added/moved About dialog
 o Version 1.6.10.0 (07.05.2013)
   - added changes to support MATLAB R2013a
 o Version 1.6.9.0 (14.10.2012)
   - added changes to support MATLAB R2012b
 o Version 1.6.8.0 (10.05.2012)
   - added changes to support MATLAB R2012a
   - fixed crash when displaying sparse variables (64 bit only)
 o Version 1.6.7.0 (21.10.2011)
   - removed MPRESS binary compression
 o Version 1.6.7.0 (22.09.2011)
   - added changes to support MATLAB R2011b
   - added 64 bit support
 o Version 1.6.6.0 (21.01.2011)
   - added support to view variables of int64 and uint64 types
 o Version 1.6.5.0 (22.09.2010)
   - added changes to support MATLAB R2010b
 o Version 1.6.4.0 (16.06.2010)
   - added changes to support MATLAB R2010a
   - changed directory of 5mat.ini from plugin directory to same directory
     as the initialization file (wincmd.ini) of Total Commander
   - fixed optional configuration of stand-alone Lister if first execution fails
   - fixed column sort order if ColumnSortOrder is not found in 5mat.ini
   - fixed calls to MAT-file API and memory consumption for packing to MAT-files
   - renamed example files
 o Version 1.6.3.0 (18.10.2009)
   - added changes to support MATLAB R2009b
 o Version 1.6.2.0 (30.03.2009)
   - added changes to support MATLAB R2009a
   - added: RAR compressed plugin without installer available
 o Version 1.6.1.0 (20.12.2007)
   - fixed Float Invalid Operation exception (Thanks to HolgerK.)
   - fixed access violation when extracting variables of mxFUNCTION_CLASS
     (Thanks to Alex T.)
   - fixed resizing and repainting of lister window
   - fixed context menu message handling of lister window
 o Version 1.6.0.1 (26.10.2007)
   - fixed: display progress bar during unpack
   - fixed example_R13SP2.mat and example_R2006a.mat
 o Version 1.6.0.0 (22.10.2007)
   - added changes to support MATLAB R2007b
   - renamed example_R2007a.mat to example_R2007b.mat
   - added printing of lister window
   - added: experimental support for timeseries objects
   - fixed: display number of variables for Version 4 MAT-files in status bar
     of lister window
   - fixed: support of multi-dimensional arrays
   - changed type of content field "Created" from ft_string to ft_datetime
   - fixed: hide read-only checkbox in file open dialog of stand-alone
     MAT-file Viewer 5mat.exe
   - fixed detection of Total Commander initialization file during
     uninstallation process
   - fixed known problem description in Readme.txt (this file)
 o Version 1.5.0.1 (02.10.2007)
   - fixed download options during installation process (could lead to loss of
     Total Commander configuration file)
   - fixed detection of Total Commander initialization file during installation
     process
   - fixed known problem description in Readme.txt (this file)
 o Version 1.5.0.0 (25.03.2007)
   - added changes to support MATLAB R2007a
   - added new example_R2007a.mat
   - renamed example_R2006b.mat to example_R2006a.mat
   - added: avoid flickering of lister window when switching from one file to
     the next (for Total Commander >= 7 beta 2)
   - added option to download MATLAB R13SP2 libs during installation process
   - fixed support of MAT-files in HDF5-based format (MATLAB >= R2006b)
   - fixed access violation when extracting creation time of MAT-files
     generated by FreeMat 3.0
   - fixed access violation when using libraries of MATLAB <= R12.1
   - fixed: load MATLAB libraries by the alternate file search strategy
   - removed DLL Diagnostic Tool 5mat_dll_diag.exe from installer
   - fixed: unlock MAT-file after failed variable viewing
   - fixed: display sparse icon for sparse complex variables
   - fixed known problem description in Readme.txt (this file)
 o Version 1.4.0.0 (15.09.2006)
   - added changes to support MATLAB R2006b
   - renamed example_R2006a.mat to example_R2006b.mat
   - added: toggle status bar using context menu (right-click in lister window)
   - fixed memory leaks of mxArray structures created by matGet...
   - fixed: any editor, lister or viewer can be used as stand-alone Lister
   - fixed: stand-alone MAT-file Viewer 5mat.exe does no longer need command
     line parameter
   - fixed: display progess bar for packing / unpacking operations
 o Version 1.3.9.0 (26.07.2006)
   - fixed creation of plugin window to support ATViewer >= 1.5.0 beta
   - fixed: pressing ESC always closes the lister window
   - added few improvements to installer 5mat_setup.exe
   - added option to download stand-alone Lister during installation process
 o Version 1.3.8.2 (06.04.2006)
   - improved support of ArchView lister plugin >= 0.9.0.0
 o Version 1.3.8.1 (30.03.2006)
   - fixed memory leaks of mxArray structures created by matGet...
 o Version 1.3.8.0 (28.03.2006)
   - added changes to support MATLAB R2006a
   - renamed example_R14SP3.mat to example_R2006a.mat
   - fixed: pressing ESC always closes the lister window
   - fixed mouse cursor handling in case of error
   - fixed memory deallocation error when browsing through multiple MAT-files
     in both Quick View mode and lister window
   - fixed known problem description in Readme.txt (this file)
 o Version 1.3.7.0 (25.11.2005)
   - added context menu (right-click in lister window)
   - fixed loading of ColumnSortOrder from 5mat.ini if ColumnSort is 3
 o Version 1.3.6.0 (02.11.2005)
   - fixed unpacking of sparse variables
   - fixed installation (5mat_setup.exe) as content plugin
 o Version 1.3.5.0 (19.10.2005)
   - fixed string allocation and memory leaks in 5mat_dll_diag.exe
   - fixed: check if return value of matGetVariable / matGetVariableInfo is NULL
   - updated installer 5mat_setup.exe to modern user interface
 o Version 1.3.4.0 (26.09.2005)
   - added simple stand-alone MAT-file Viewer 5mat.exe
 o Version 1.3.3.0 (19.09.2005)
   - added column sorting in the lister view
 o Version 1.3.2.0 (04.09.2005)
   - added changes to support MATLAB R14SP3
   - renamed example_R14SP2.mat to example_R14SP3.mat
 o Version 1.3.1.0 (31.07.2005)
   - added optional status bar to lister view
   - fixed exception handling for matGetVariableInfo
   - fixed uninstaller.exe does not delete 5mat.ini any longer
   - renamed DLL Check & Download Tool 5mat_dll_check.exe to DLL Diagnostic
     Tool 5mat_dll_diag.exe
 o Version 1.3.0.0 (10.07.2005)
   - added lister view
   - change: use own 5mat.ini (instead of default xxplugin.ini)
   - fixed detection of mxLOGICAL_CLASS
   - fixed unpacking of logical, sparse variables
   - fixed size display of number of elements (row and column size needed to be
     swapped)
   - removed support of CanYouHandleThisFile()
 o Version 1.2.0.0 (22.06.2005)
   - added display configuration of variable class and number of elements for
     extension column of Total Commander using the configuration dialog
   - added content information
   - added SaveFileAs dialog for Download Tab of DLL Check & Download Tool
     5mat_dll_check.exe
   - fixed packing to MAT-files if MAT-file has to be created
   - fixed packing to MAT-files if variable names are identical
 o Version 1.1.1.0 (02.06.2005)
   - added DLL Check & Download Tool 5mat_dll_check.exe to verify and solve DLL
     dependencies
   - installation is automatically done by installer 5mat_setup.exe
   - fixed DLL freeing in case of error
 o Version 1.1.0.0 (28.05.2005)
   - added support for compressed and Unicode encoded MAT-files of MATLAB >= R14
   - enhanced description of MATLAB dependencies in Readme.txt (this file)
 o Version 1.0.2.0 (19.04.2005)
   - fixed size information for empty fields of mxSTRUCT_CLASS
   - fixed display problem for empty fields of mxSTRUCT_CLASS
 o Version 1.0.1.1 (11.04.2005)
   - fixed display indent problem for empty fields of mxSTRUCT_CLASS
   - fixed variable deletion
 o Version 1.0.1.0 (07.04.2005)
   - added information of the variable class and number of elements in extension
     column of Total Commander
   - fixed display indent problem for mxCELL_CLASS and mxSTRUCT_CLASS
   - corrected limitations in Readme.txt (this file)
   - added extended example MAT-files
   - added Descript.ion file (for use with TC Plugins Manager >= 1.6.0)
 o Version 1.0.0.4 (11.02.2005)
   - added simple configuration dialog for number of displayed digits
   - added exception handling for unpacking of variable classes mxINT64_CLASS,
     mxUINT64_CLASS, mxFUNCTION_CLASS, mxOPAQUE_CLASS and mxOBJECT_CLASS
   - added unpacked size information for variable classes mxCELL_CLASS,
     mxSTRUCT_CLASS, mxLOGICAL_CLASS and mxFUNCTION_CLASS
   - fixed unpacking of empty array, cell, field and string variables
   - ignoring zero imaginary part of complex variables
   - added support for CanYouHandleThisFile()
 o Version 1.0.0.3 (31.01.2005)
   - removed usage of string class
   - added support for viewing structs, cells, sparse and complex variables
   - added simple check for overwriting a variable in case Total Commander
     can not handle this
   - added viewing options for number of digits when displaying double,
     single or sparse data using pkplugin.ini
 o Version 1.0.0.2 (29.01.2005)
   - removed some superfluous variables
   - added check for success of malloc()
   - restructured parts of code
 o Version 1.0.0.1 (27.01.2005)
   - first public version


 * References:
--------------

 o The MathWorks External Interfaces
   - http://www.mathworks.com/help/pdf_doc/matlab/apiext.pdf
 o The MathWorks External Interfaces Examples
 o The MathWorks MAT-file Format
   - http://www.mathworks.com/help/pdf_doc/matlab/matfile_format.pdf
 o Content-Plugin Writer's Guide by Christian Ghisler
   - http://ghisler.fileburst.com/content/contentpluginhelp2.1.zip
 o LS-Plugin Writer's Guide by Christian Ghisler
   - http://ghisler.fileburst.com/lsplugins/listplughelp2.1.zip
 o WCX Writer's Reference by Christian Ghisler & Jiri Barton
   - http://ghisler.fileburst.com/plugins/wcx_ref2.21se.zip


 * Acknowledgments:
-------------------

 o Bug reports, suggestions and testing by HolgerK
 o CPrintListCtrl by Marquet Mike
   - http://www.codeguru.com/cpp/controls/listview/printing/article.php/c1003
 o Debugging and testing by Christian Ghisler
   - http://www.ghisler.com


 * Trademark and Copyright Statements:
--------------------------------------

 o MATLAB is a registered trademark of The MathWorks, Inc. Other product or brand
   names are trademarks or registered trademarks of their respective holders.
   - http://www.mathworks.com
 o Total Commander is Copyright © 1993-2013 by Christian Ghisler, Ghisler Software GmbH.
   - http://www.ghisler.com


 * Feedback:
------------

If you have problems, questions, suggestions please contact Thomas Beutlich.
 o Email: support@tbeu.de
 o URL: http://tbeu.totalcmd.net