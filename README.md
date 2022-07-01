wmutils' core
=============

wmutils' core is a set of tools for X windows manipulation. Each tool only has
one purpose, to make it as flexible and reliable as possible.

Here's a quick example. This snippet will put the currently focused window in
the middle of the screen:

    #!/bin/sh

    # get current window id, width and height
    WID=$(pfw)
    WW=$(wattr w $WID)
    WH=$(wattr h $WID)

    # get screen width and height
    ROOT=$(lsw -r)
    SW=$(wattr w $ROOT)
    SH=$(wattr h $ROOT)

    # move the current window to the center of the screen
    wtp $(((SW - WW)/2)) $(((SH - WH)/2)) $WW $WH $WID

You also might want to check [sxhkd](https://github.com/baskerville/sxhkd), a
daemon which allow binding applications to hot keys.

utilities
---------

* chwb  - change window's border
* chwso - change window's stacking order
* ignw  - ignore/unignore window
* killw - kill windows
* lsw   - list windows
* mapw  - map/unmap windows
* pfw   - print focused window
* pdw   - print decoration window
* slw   - select window interactively
* wattr - show window's attributes
* wmp   - move the mouse pointer
* wmv   - move a window
* wrs   - resize a window
* wtf   - focus a window
* wtp   - teleport a window
* atomx - modify atoms on a window

All these tools come with a manpage ! read them for further informations.

dependencies
------------

wmutils only relies on the XCB library.

license
-------

All the code of this project is released under the
[ISC](http://www.openbsd.org/policy.html) license. See LICENSE file.

build & install
---------------

System-wide installation (default PREFIX is `/usr`):

    $ make
    # make install

Here are the variables you can override, along with their default values:

    CC         =  cc
    LD         =  $(CC)
    CFLAGS     =  -std=c99 -pedantic -Wall -Os
    LDFLAGS    =  -lxcb
    PREFIX     =  /usr
    MANPREFIX  =  $(PREFIX)/man
    DESTDIR    =

You can override them by passing them on the command line:

    make DESTDIR=/newroot install

Or by setting them via the environment:

    CC=c99; export CC
    make -e

contribute
----------

You can contribute to the project in many ways; be it by testing the software,
reporting bugs, submitting patches, or simply talking about it to other
people!
The official IRC channel for wmutils' talks is #wmutils @irc.freenode.net.

If you find a bug in the software, please report an issue. Same goes for
feature requests, if they match the project's philosophy.

When submitting pull requests, please reference the issue number. If there is
no issue to reference, then create one and wait for a validation of the bug
before submitting the pull request.

IF YOUR PULL REQUEST DOESN'T REFER TO A VALID ISSUE, IT WILL BE DISCARDED.
