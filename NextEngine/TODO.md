
## TODO
 - Ability to remove textures from framebuffers
 - Spotlight broke
 - Make ScreenQuad support multisampled textures with texelFetch
 - Multiple windows
 - Automatically generate tangents if none are available
 - Expose DropTarget API and let users decide what they want to be droppable
 
 ## COMPLETE
 - Fix framebuffer memory leak					: Forgot to call glDeleteFramebuffers();
 - Make texture class support frame textures	: Made a texture class that replaces both
 - Switch texture formats to internal formats	: ^Said texture class uses both formats for more control

 ## ON HOLD
 - Move render system and device from namespace to class for multithreading		: I did this, but OpenGL is a pain with MT, so wait until Vulkan to do this
