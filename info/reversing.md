# Overview
So looking for info on the VST 2. It looks like it's non-longer maintained by the original creator, and they are pushing VST 3. VST 2 also seems to have some pretty harsh licensing from a quick search about it. Also sounds like the creator is no long licensing the SDK. Just going to avoid this all together. So I am gonna try to figure out the structure for VST 2 and write a C and Rust interface for them. Also put my reverse engineering skills to work.

# Findings and Process
The below will be me documenting what I have figured out, and how.

## Functions
DLLS Seem to export 2 functions *main*, and *VSTPluginMain*. The fact it so few means these functions probably have pretty complicated parameters. Also the fact *main* and *VSTPluginMain* most the time seem to have the same offset means they should have the same function signature.

![VST 2 Functions](./images/funcs.png)

## Function Signatures
So looking at the assembly of 64 bit vs. 32 bit dlls the return values differ. On windows a plain int is 32 bits. So it's not returning an int on 64 bit. It's either a large number or a pointer. I am going to probably guess returning a pointer, as it would make no sense to change the width between 32bit and 64bit. The 64 bit dlls so far I have looked at only take a single pointer as an argument, but 32 bit dlls are not remaining constant. At least when looking at decompiled code from snowman. Different argument numbers would not make sense for a dll entry point. So I need to look more closely at the assembly, and the call stack.

![VST 2 Main](./images/funcs2.png)

So far from looking more closely it seems like it should be only one argument. Also noticed an other thing with snowman it does not account for the registers as part the windows 64 bit calling convention in all cases. For instance looking at the assembly here it's pretty obvious.

![Snowman Fail](./images/funcs3.png)

However, the VST 2 entry point seems to take one argument, which is a call back that takes 6 six arguments. I am currently wondering if it's something like wglGetProcAddress() which is a function that lets you get other functions. Not unusual design choice for systems that can add new function calls. However just a tentative guess at this moment.

Well after a little more static analysis, it looks like the entry point returns a pointer, to what I think is a struct or array of some kind. However, still have not really been able to determine the arguments, or what the fields are to this struct. Probably going to need to load the plugins and see how the application and program interact.

So I am gonna use LMMS to launch the VST plugins, it spawns a seperate process for each VST. (RemoteVstPlugin.exe and RemoteVstPlugin32.exe) So I need to attach my debugger to this process as soon as it starts. Luckily, I can create a *HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options* to make this easy on windows, but...

Well doing that seems to lockup LMMS, and the *RemoteVstPlugin* executable will terminate itself after letting run. So I have written a very simple program that blindly polls the processes and suspends one the VST Plugin processes. (See suspender.c in the tools folder) This gives me plenty of time to attach debugger and then resume the process, and it seems to work well enough yay! (やった!) This will be much more effective now that I can examine the DLLs under use.

![Debugger Success](./images/debugger.png)

# Return Type Struct Pointer or String Pointer?

So the returned pointer is in the data segment of this dll. However, I thought it was a struct when looking around with snowman although it looks to be just the string "PtsV". Also seems be the same in other dlls. Maybe *VSTPluginMain* does not return anything that interesting, unless the data following afterwards is used by the calling the calling process. If it's just a string I am I probably done here.

![just a string?](./images/string.png)

So the host process does multiple things with the returned pointer. It checks that DLL does not return *NULL*. Then it checks to see if the pointer points to a value equal to "PtsV". If it does not equal "PtsV" it does not consider the DLL a VST plugin. This value seems to be used so one can indentify a VST plugin. However, it looks like the pointer is accessed at different offset when looking at assembly code ahead. So clear it's some kinda of structure that begins with a string.

![A struct with a string](./images/string2.png)
