lst2str v1.20
---------------------------------
[28.04.05] Version 1.20

WARNINGS (!!!):
  * You can now define your prefered command line length, however, be warned, some programs [winamp ...] cannot handle more than the good old 256 bytes, so to conserve valuable space use short DOS names, in TCmd's case use lowercase switches (%l, %m etc..)
  * Please note that if you use the %T%M switch from TC, the aditional [file] will be passed even if it's not marked -> DON'T use it unless you must.

History:
  Changes v1.20 [28.04.05]:
    [?] Changed the way CreateProcess is called (again).
    [?] Minor internal changes.
    [?] Fixed an issue with strings not being parsed correctly leading to garbage.

  Changes v1.19.1 [24.04.03]:
    [?] Fixed a slight issue with the CreateProcess; (the /S parameter wasn't working correctly)

  Changes v1.19 [20.04.03]:
    [+] Added a new switch /E (for CL length warnings).

  Changes v1.18 [13.04.03]:
    [+] Added a cap of ~32k to CL length (windows doesn't support more).
    [?] Minor bug fixes.

  Changes v1.17 [12.04.03]:
    [?] Removed the 4096 cap on CL, default is still 256.
    [?] Minor optimizations.

  Changes v1.16w [19.03.03]:
    [?] No actual changes, just recompiled it as a windows app., this should rid you of the console window popping up.

  Changes v1.16 [17.03.03]:
    [?] Changed the command line length behaviour. (See iix)
    [?] Rewrote the parsing algorithm -> faster, smaller and WAY meaner.
    [?] Removed the "heavy" includes.
    [?] Changed the compiler to MinGW with gcc.
    [?] All the changes made it possible for the executable to become a mere 6kb!!.

  Changes v1.15 [12.03.03]:
    [+] Added a new switch /W (controlls window state).
    [+] Added a new switch /C (command line length limit).
    [+] Added error announcements through MessageBox.
    [?] Changed the way executables are called (to CreateProcess).
    [?] Changed the priorities of /F, /L.

  Changes v1.11 [10.03.03]:
    [?] Changed the [file] parameter handling, it's now passed last (after the [listfile]).

  Changes v1.10 [07.03.03]:
    [+] Added better switch handling.
    [+] Added another file parameter.
    [+] Added a cap for number of files to pass.
    [+] Completely rewrote parameter handling.

  Initial release v1.01 [11.01.03]

1. Usage:
  lst2str /X:[program.exe] /L:[listfile] /F:[file] /N:[nnn] /W:[state] /C:[nnn] /S:[switches]

  /X:[program.exe] : Program name you would like to run. (NOT optional)
  /L:[listfile   ] : The list file. (NOT optional)
  /F:[file       ] : An added file, it will be passed last, and only if /N allows it {a fix for TCmd target pane file}. (default: NONE)
  /N:[nnn        ] : An integer specifying the maximum number of files to pass, such as ncompare or examdiff. (default: "INFINITE")
  /W:[state      ] : One of the following {M, m, H} where [M] is maximized, [m] minimized and [H] hidden. (default: will open the program in it's default state)
  /C:[nnn        ] : An integer specifying the limit for command line length, can be in the following range [256-(2^16 - 1)]. (default: 256)
  /S:[switches   ] : Switches to pass to the program BEFORE the contents of [listfile] and the [file]. (default: NONE)
  /E               : If specified, lst2str will output a warning if the input contains more data than CL should hold (OK->continue / CANCEL->exit). (default: no warning AND continue)

2. Notes:
  (i  ) If the [listfile] contains directories ending with [\], they will be stripped. This was done mainly for winamp.
  (ii ) If the /C parameter is out of bounds the closest bound will be taken, i.e. /C:112 --> /C:256.
  (iii) The /X: and /L: parameters are NOT optional, everything else IS.
  (iv ) Every parameter (except those with integers or chars) should be enclosed in quotes ["]; TCmd passes quotes when needed, in these cases no need to enclose the parameter.
  (v  ) The switches parameter can be "-s1 -s2 -s3" i.e. several switches; [switches] parameter is passed to the program without quotes so if you put /S:"my program.exe" inside it'll be interpreted as to parameters "my" and "program.exe".
  (vi ) The parameters can be passed in any order you want.
  (vii) If [file] is specified it'll be passed only if /N allows it, i.e. /L takes priority.
  (iix) If the command line length exceeds the allowed value only the allowed ammount will be sent to the program. For example the command line is ["abc" "def" "ghi"] which amounts to 18 chars, now say I only allowed 12 chars -> the output would be ["abc" "def"], 11 chars.

3. Example:
  lst2str /X:"c:\program files\winamp\winamp.exe" /L:%L /S:"/ADD" /W:m /E
  This would start winamp (minimized) with the /ADD switch passing the contents of the [listfile] and if the data in the list file is longer than 256 will output a warning.

  lst2str /X:"c:\program files\uedit\uedit32.exe" /L:%L /F:%T%M /N:2 /C:2048
  This would start UltraEdit with the files in [listfile] up to 2 and if it hasn't reached the limit adds the [file] parameter, while limiting the comand line length to 2048.

Comment:
  Thanks a bunch for constructive suggestions on [Total Commander] board.