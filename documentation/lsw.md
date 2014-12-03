NAME
----
lsw - list child windows

SYNOPSIS
--------
    lsw [-air] [wid..]

        -a : list all windows (both mapped and unmapped)
        -i : include ignored windows (cf. ignw(1))
        -r : print root window id

If no window id is given, lsw will return all child windows of root window.

ENVIRONMENT
-----------
lsw follows the variable `DISPLAY` to set X.org display to act on.

SEE ALSO
--------
mapw(1), ignw(1), xargs(1)
