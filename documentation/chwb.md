NAME
----
chwb - change window borders

SYNOPSIS
--------
    chwb [-sc <arg>] <wid> [wid..]

        -s <arg> : set border width of <wid> to <arg>
        -c <arg> : set border color of <wid> to <arg>

COLOR SPECIFICATION
-------------------
Colors should be passed in hexadecimal value, as specified by strtoul(3)
(namely, "rrggbb").

ENVIRONMENT
-----------
chwb follows the variable `DISPLAY` to set X.org display to act on.

SEE ALSO
--------
strtoul(3)
