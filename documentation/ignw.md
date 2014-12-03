NAME
----
ignw - ignore window

SYNOPSIS
--------
    ignw -sr <wid> [wid..]

        -s : set override_redirect value to 1 for <wid> (ignore window)
        -r : set override_redirect value to 0 for <wid> (unignore window)

ENVIRONMENT
-----------
ignw follows the variable `DISPLAY` to set X.org display to act on.

SEE ALSO
--------
lsw(1), xcb_get_window_attributes(3)
