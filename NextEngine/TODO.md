
## TODO
- [ ] Primitive render flags for hasNormals, hasTangents, acceptsLighting, etc...
- [ ] Fix negative scaling in matrix decomposition
- [ ] Make ScreenQuad support multisampled textures with texelFetch
- [ ] Multiple windows
- [ ] Expose DropTarget API and let users decide what they want to be droppable
- [ ] Full GLTF file support
    - [ ] Skins
    - [ ] Morph targets
    - [x] Multiple scenes
    - [x] Sparse Accessors
    - [x] Local node transforms
    - [x] Animations
    - [x] Vertex Colors
    - [x] Normals
    - [x] Textures
    - [x] Meshes
    - [x] Primitives
 
 ## COMPLETE
- [x] Fix spot light
    - Normals are inverted. Some models don't have normals/tangents, so ligthing doesn't work properly in it's current state.
- [x] Animation rework
    - See <src/nxt/world/Animation.h> for details
- [x] Add multiple animation tracks
- [x] Fix buffer data copying always putting INVALID_VALUE error.
    - Sparse accessor value buffers don't have a buffer target. When DataBuffers are initialized, they are bound to their target and data is pushed into that target. Because no target was set, the target is 0 and the data goes nowhere.
    - Fixed by using direct storage access
- [x] Fix heirarchy model primitives all using the same vertex data
    - Added array objects to each primitive. Previously it was per model, so all primitives were using the same data.
- [x] Ability to remove textures from framebuffers
- [x] Adjust SSAO matrices.
    - Rebuilding viewspace & worldspace positions through depth buffer
    - Normal matrix has been corrected
- [x] Fix deferred depth buffer
    - Never actually added a depth attachment to the deferred buffer
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
- [ ] Automatically generate tangents if none are available
- [ ] Move render system and device from namespace to class for multithreading
