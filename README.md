wmutils' core
=============

wmutils' core is a set of tools for X windows manipulation. Each tool only has
one purpose, to make it as flexible and reliable as possible.

Here's a quick example of what can be achieved with wmutils, this screen will
put the currently focused window in the middle of the screen:

    ```sh
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

utilities
---------

Here are the tools. This project is quite young, new tools can arrive
without being added to this list, so take it with a grain of salt.

* chwb  - change window's border
* chwso - change window's stacking order
* ignw  - ignore/unignore window
* killw - kill windows
* lsw   - list windows
* mapw  - map/unmap windows
* pfw   - print focused window
* wattr - show window's attributes
* wmp   - move the mouse pointer
* wmv   - move a window
* wname - get a window's name
* wrs   - resize a window
* wtf   - focus a window
* wtp   - teleport a window

All these tools come with a manpage ! read them for further informations.

dependencies
------------

wmutils only relies on the XCB library.

build & install
---------------

System-wide installation (default PREFIX is `/usr`):

    $ make
    # make install

Fakeroot installation:

    $ make
    $ make DESTDIR=$PWD/newroot install
