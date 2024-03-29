# winterchange
An abstract window interface, designed to allow an application to interact with a window without depending on the implementation of that window's display, allowing for minimal changes to the dependent application.

An application using winterchange should be able to construct specific windows that implement the Window interface (implementations which are provided by winterchange), then interact with those windows only via the methods available through the Window interface.

Currently the only Window implementation built against the interface is a wrapper on around ncurses/PDcurses.

## License - MPL v2.0

winterchange is licensed under the terms of the Mozilla Public License v2.0, available in LICENSE.txt.

## Building

Currently do not have a IDE-independent build process that just uses make files. May add down the road.

Winterchange can be built in a Debug configuration, which creates an executable that will run tests in main.cpp, or a Release
configuration, which builds winterchange as a dynamic library.

### Mac OS X

Pre-requisites:
- Eclipse CDT (make file is generated by eclipse project at the moment)
- Assumes g++ (or g++ symlinked to clang/LLVM) is installed.
- Assumes gdb is installed for debugging
- ncurses dynamic library available on the system

1. In a new or existing Eclipse CDT workspace, Right-click in the Project Explorer or Navigator and choose `Import...`
2. From the import wizard, choose General -> Existing Projects into Workspace
3. In Select Root Directory, browse to the folder to which you cloned winterchange
4. Ensure the winterchange project is checked in the list, and click Finish

#### Building to Debug (default) 
1. Click on winterchange in the project explorer and choose Properties.
2. Click C/C++ Build in Project Properties, then click Manage Configurations in the pane to the right and ensure Debug is set as the active configuration.
3. Click on winterchange in the project explorer and choose Build Project (or Build All, or Cmd-B) from the Project Menu

If build is successful, `winterchange` executable should be placed in Debug subfolder of the project root folder.

#### Building for Release 
- Follow the steps above for Debug, except set Release as the active configuration.

If build is successful, `libwinterchange.dylib` library should be placed in the Release subfolder of the project root folder.

### Windows

Pre-requisites:
- Visual Studio 2022 Community Edition.

1. Go to File -> Open -> Project/Solution
2. Browse to the folder to which you cloned winterchange, and choose the `winterchange.sln` file, and click Open.
3. Go to Build and choose Build Solution or Build winterchange with the winterchange project selected

If build is successful, `winterchange.exe` should be placed in Debug sub folder of project root folder.

## Linking against winterchange

TBD
