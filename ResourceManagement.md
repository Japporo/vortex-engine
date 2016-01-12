# Introduction #


# Details #

Resources in Vortex engine are contained in package files (PAK). The layout of these files are described in detail on the Maelstrom tool wiki [Here](http://code.google.com/p/maelstrom-editor/wiki/ContentPackage).

Resources are loaded in Vortex by suppling the ResourceManager with a package name and the resource identifier (a unique integer value).
If the package has been loaded previously it is fetched from the package cache. Vortex will not unload a package unless specifically told to do so... Thats not good. I will rethink that right away! :)