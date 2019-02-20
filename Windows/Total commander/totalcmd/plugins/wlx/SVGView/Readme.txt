SVGView 1.2.8.1
Plugin for TC for view SVG files.

It can use RSVG (rsvg-convert.exe) or ImageMagic for convert SVG files to bitmap.

Unpacked version of RSVG (from http://opensourcepack.blogspot.ru/2012/06/rsvg-convert-svg-image-conversion-tool.html) used by default and included to archive.
For open SVG files with Unicode symbols plugin copy these files to temp folder and open it.
If you don't want this behaviour then switch off it in ini:
[Setup]
UseRSVGHelper=0

For ImageMagick support download it from
http://www.imagemagick.org/script/binary-releases.php#windows
put files to subdir in plugin directory
ImageMagick_x32\
for 32-bit ImageMagick files, and
ImageMagick_x64\
for 64-bit ImageMagick files and switch off RSVG:
[Setup]
UseRSVG=0


License agrement
This software provided "AS-IS" without warranty of any kind for non-commercial use only.


Changes history

Ver 1.2.1:
 * public release.

Ver 1.2.2:
 * new rsvg-convert.exe version;
 + OpenWidth= in INI [Setup] section: set width for open image (10-10000, 0 is default);
 + OpenHeight= in INI [Setup] section: set height for open image (10-10000, 0 is default).

Ver 1.2.3:
 * fixed bug of image draw in TC thumbnail mode.

Ver 1.2.4:
 * try use short name first for Unicode filenames;
 * possible zero bytes file size crash fixed.

Ver 1.2.5:
 + try check svgz is zip and contain svg with same name;
 + RSVGPath= in INI [Setup] section: set full path to rsvg converter.

Ver 1.2.7:
 * fix for try use short name first for Unicode and very long filenames;
 + "Reload Image" menu item;
 * some internal changes.

Ver 1.2.8:
 * converter RSVG-Convert updated to version 2.40.20;
 + background color;
 + HighPriority=1 in INI [Setup] section: set high priority for converter;
 + ImageMagickPath= and ImageMagickPath64= in INI [Setup] section: set full path to ImageMagic converter;
 + ImageMagicAdditionalCmd= and RSVGAdditionalCmd= in INI [Setup] section: additional command line parameters for converter;
 * internal changes.

Ver 1.2.8.1:
 + ShowThumbnailsInTC=1 in INI [Setup] section: plugin can get thumbnails for TC;
 + ShowLoadingTime=1 in INI [Setup] section: show image loading time;
 + InternalConvertPNGtoBitmap=0 in INI [Setup] section: convert png image to bitmap before show for avoiding zoom errors;
 * some internal changes.


---
Suggestions, Wishes and bug reports are welcome!
ProgMan13, (ProgMan13@mail.ru)