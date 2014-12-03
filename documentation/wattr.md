NAME
----
wattr - get windows attributes

SYNOPSIS
--------
    wattr [bmwhxy] <wid>

        b : print <wid> borderwidth
        x : print <wid> X offset
        y : print <wid> Y offset
        w : print <wid> width
        h : print <wid> height
        m : returns 0 if window is mapped on screen, 1 otherwise

When run without querying an attribute, `wattr` will return 0 if the window
exists, 1 otherwise.

Attributes can be chained, they will be printed in the order specified,
separated by spaces.

EXAMPLE
-------
    $ wattr m 0x01000006 && echo ismapped
    ismapped

    $ wattr whxy 0x01000006
    484 244 756 166

    $ wattr xyhw 0x01000006
    756 166 244 484

    $ wattr 0x00000000
    $ echo $?
    1


ENVIRONMENT
-----------
wattr follows the variable `DISPLAY` to set X.org display to act on.
