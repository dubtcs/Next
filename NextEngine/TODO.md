
## TODO
- [ ] Ability to remove textures from framebuffers
- [ ] Make ScreenQuad support multisampled textures with texelFetch
- [ ] Multiple windows
- [ ] Automatically generate tangents if none are available
- [ ] Expose DropTarget API and let users decide what they want to be droppable
 
 ## COMPLETE
- [x] Fix model drag and drop
    - DrawCube ArrayObject buffers were being overwritten when loading new files with drag&drop. Call stack is [ WindowProc() -> Update() -> RenderPasses() -> DrawViewport() ]. Because the ArrayObject wasn't unbound at the end of the Viewport() call, it's contents were being overwritten by the WindowProc() loading a new model at the start of the next tick.
- [x] Add normal maps to deferred
- [x] Spotlight broke
    - Forgot to convert to radians and apply cosine.
- [x] Fix framebuffer memory leak
    - Forgot to call glDeleteFramebuffers();
- [x] Make texture class support frame textures
    - Made a texture class that replaces both
- [x] Switch texture formats to internal formats
    - New texture class uses both formats for more control

 ## ON HOLD
- Move render system and device from namespace to class for multithreading		: I did this, but OpenGL is a pain with MT, so wait until Vulkan to do this
