# AndroidCore-V1 ( Im going to be actively working on this until i find something else to do. )

Nothing much just a 32bit open source Android Executor for people who has small knowledge on modding for Roblox Android like myself. This will be badly coded since i'm not perfectly good at modding Roblox. So i wouldn't recommend using this as your base or what. This uses Visual Studio 2019

**This only has few basic custom functions which isn't good if you want to make Android Executor..**

Addresses is outdated and can be located on ``update/update.hpp`` which can be used to update this executor.

To inject in Roblox, get the library find Roblox's APK folder ``lib/`` Now decompile the Roblox's classes.dex and go to ``com/roblox/client/ActivityNativeMain``

In there find the ``OnCreate`` method and just below it Paste this

```
const-string v0, "AndroidCore-V1"
invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```

```
This exploits contains
getgenv
identifyexecutor
getthread
getrenv
getreg
isLuau
isrbxactive
```

# TODO
* Exploit the taskscheduler
* Add file system
* Better way to exploit scriptcontext
* Render files
* Register the functions
* Clean up

**Some of this codes are from the guy named nop**
