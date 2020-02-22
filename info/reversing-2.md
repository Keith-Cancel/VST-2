# Sections
[Getting Started (Part 1)](./reversing-1.md)

[Reversing Functions (Part 2) - *here*](./reversing-2.md)

# Functions

So we have identities in 7 functions. The callback provided to *VSTPluginMain*, and the 6 functions in the struct. The struct has 3 values that could be pointers as well that uses eludes me at the moment. So the simplest order of bushiness is to set break points on each function and see how the args change with each call, and observe the behavior.

# Function Number One

So I shall start with the sole argument to *VSTPluginMain* we already know that it has 6 arguments. So lets try to finish what we started. So the first time it the function is called it args are 0, 1, 0, 0, 0, and 0.

## Parameter One

Well that was an easy one. So far other than first call which is zero or NULL it has always been a pointer to our struct we spent time dissecting earlier.

![Arg One](./images/func1-arg1.png)

## Parameter Two

Well this function seems have suddenly gotten more complicated. When looking at what LMMS does with this value we can see that it's being used to drive a jump table. Also looking where the function returns it's quite large. So this function is behaving more like many functions. Moreover, from a quick look this seems to determine even if the other arguments/parameters are used or not.

![Arg Two](./images/func1-arg2.png)