NAME
----
wtp - teleport window (absolutely)

SYNOPSIS
--------
    wtp <x> <y> <w> <h> <wid>

        x : X offset to set for <wid> 
        y : Y offset to set for <wid> 
        w : width to set for <wid>
        h : height to set for <wid>

This command use absolute values, and teleport `<wid>` to this geometry.

ENVIRONMENT
-----------
wtp follows the variable `DISPLAY` to set X.org display to act on.
