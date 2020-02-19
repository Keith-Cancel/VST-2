# Overview
So looking for info on the VST 2. It looks like it's non-longer maintained by the orginal creator, and they are pushing VST 3, both which seems to have nasty licensing. Just going to avoid this all together. So I am gonna try to figure out the struture for VST 2 and write a C and Rust interface for them.

# Functions
DLLS Seem to export 2 functions main, and VSTPluginMain. The fact it so few means these functions probably have pretty complicated parameters. Also the fact main and VSTPluginMain most the time seem to have the same offset means they should have the same function signature.

![VST 2 Functions](./images/funcs.png)

# Function Signatures