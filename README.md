
wmutils' core
=============

wmutils' core is a set of tools for X windows manipulation. Each tool only has
one purpose. We follow the UNIX philosophy: Do one thing and do it well.

why?
----

There are many window managers out there, each of them having their own strengths and
weaknesses. Instead of writing another WM that would suit our exact needs, we
decided to provide tools that everyone could use to match their exact needs in regards
to window management.

how?
----

wmutils are written using the XCB library. Each source file provides a tool which
is part of the core. You can then use these tools either from the command line,
or wrap them in scripts and use keybind programs (like skhxd(1) or xbindkeys(1))
to spawn those scripts. Possibilities are endless.

what?
-----

Here are the tools. This project is quite young, new tools can arrive
without being added to this list, so take it with a grain of salt.

* chwb  - change window's border
* chwso - change window stack order
* killw - kill windows
* lsw   - list windows
* mapw  - map/unmap windows
* pfw   - print focused window
* wattr - get a window's attributes
* wmv   - move windows
* wrs   - resize windows
* wtf   - focus windows
* wtp   - teleport window

examples
--------

    # move the current window by 40 pixels to the right
    $ wmv 50 0 $(pfw)

    # hide all windows
    lsw | xargs mapw -u

    # kill all hidden windows
    (lsw;lsw -a) | sort | uniq -u | xargs killw

    # focus lowest window in the stacking
    next=$(lsw | grep -v $(pfw) | sed 1q)
    test -n "$next" && {
        wtf $next
        chwso -r $next
    }

    # put current window in the top-left corner
    wtp 0 0 $(wattr wh `pfw`) $(pfw)

    # focus next window and set it as active
    next=$(lsw|grep -v $pfw)|sed 1q)
    lsw | xargs chwb -s 4 -c 0
    chwb -s 4 -c 0xff0000 $next
    wtf $next

Just let your imagination flow through you...

build & install
---------------

You will need the xcb library to build these tools. Check your distribution's
packages to find how to install it.

    # build a single tool
    make <tool>

    # build them all
    make all

    # install them
    make install
