
wmutils' core
=============

wmutils' core is a set of tools for X windows manipulation. Each tool only has
a single purpose, in order to one thing and do it well.

why?
----

There are many window managers out there, each of them having their strengh and
weakness. Instead of writing another WM that would suit our exact needs, we
decided to follow the Unix philosophy, and provide tools that everyone could use
to match their exact needs about window management.

how?
----

wmutils are written using the XCB library. Each source file provide a tool which
is part of the core. You can then use these tools either from the command line,
or wrap them in scripts and use keybind programs (like skhxd(1) or xbindkeys(1))
to spawn those script. Possibilities are endless.

what?
-----

Here is the list of tools. This project being quite young, new tools can arrive
without being added to this list, so take it with a grain of salt.

* chwso - change window stack order
* killw - kill windows
* lsw   - list windows
* mapw  - map/unmap windows
* pfw   - print focues window
* wattr - get window's attributes
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

Just let imagination flow through you...

build & install
---------------

You will need the xcb library to build these tools. Check you distribution
packages to find how to install it.

    # build a single tool
    make <tool>

    # build them all
    make all

    # install them
    make install
