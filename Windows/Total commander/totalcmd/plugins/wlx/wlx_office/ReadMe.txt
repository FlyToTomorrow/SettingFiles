MS Office file viewer plugin for Total Commander

This plugin is intended for viewing MS Word, Excel, Write and other files for
which a convertor into RTF format is installed and configured in the office.ini
file. Convertors for Word, Excel, Write and HTML formats come with MS Office and can
be found in \Program Files\Common Files\Microsoft Shared\TextConv directory.

Installation
------------
1. Unzip the plugin to a directory of your choice.
2. In Totalcmd, view any file with F3
3. Choose Options - Configure - LS-Plugins
3. Click on "Add"
4. Choose the .wlx file of that plugin and click OK

Then, if necessary, edit convertor paths in the office.ini file. Restart Totalcmd.

You can now access the new lister plugin with F3.

office.ini file format
--------------------------
Each line in the office.ini file describes one format/converter pair, and has the
following format:

N=Ext,Signature,Convertor_file

Here N is a sequential number (starting from 0, no gaps!), Ext is the file extension, Signature (optional)
is some first bytes of the file in hex or quoted string format, and Convertor_file is a full path to
the convertor file, or a convertor file name only. In the last case, the plugin looks for the convertor in
the plugin's directory, and if not found, in the directory where MS Office has registered the convertor.
The convertors are tried in the order defined by N, until one is found that recognizes the format. Examples:

0=doc,D0CF11E0A1B11AE1,mswrd832.cnv
1=doc,D0CF11E0A1B11AE1,C:\Program Files\Common Files\Microsoft Shared\TextConv\mswrd632.cnv
2=htm,"<",C:\Program Files\Common Files\Microsoft Shared\TextConv\html32.cnv

First line assigns a convertor for MS Word files (extension is "doc"). Files of this format
(actually, all MS compound files!) start with the following bytes (in hex): D0, CF, 11, E0, A1,
B1, 1A, E1, so the signature is D0CF11E0A1B11AE1. The 2nd line assigns another convertor, just
in case the first one does not recognize file format. The 3rd line assigns HTML convertor to
.htm files starting with "<" (without quotaion marks).

Disclaimer
----------
Released as is to the public domain, along with source code. The author have absolutely no time for
supporting the plugin, and can not and will not guarantee anything.

Known problems and Limitations
------------------------------
- RTF convertor may be pretty slow, especialy if the file being converted contains much graphics.
- For multisheet XLS files, Excel converter asks what spreadsheet to convert.
- Line/word wrapping in tables is not supported. This is a MS RichEdit control limitation. Because
  of this, support for Excel files is pretty poor.

Contact the author
------------------
Yuri Gershanov
