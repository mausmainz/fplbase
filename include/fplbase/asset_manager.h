// Copyright 2014 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef FPLBASE_ASSET_MANAGER_H
#define FPLBASE_ASSET_MANAGER_H

#include <map>
#include <string>

#include "fplbase/config.h"  // Must come first.

#include "fplbase/async_loader.h"
#include "fplbase/fpl_common.h"
#include "fplbase/renderer.h"
#include "fplbase/texture_atlas.h"

namespace fplbase {

/// @file
/// @addtogroup fplbase_asset_manager
/// @{

/// @class FileAsset
/// @brief A generic asset whose contents the AssetManager doesn't care about.
class FileAsset : public AsyncAsset {
  virtual void Load();
  virtual void Finalize();
 public:
  std::string contents;
};

/// @class AssetManager
/// @brief Central place to own game assets loaded from disk.
///
/// Loads of some assets, such as textures, can be batched in a bit asynchronous
/// load. This allows you to continue setting up your game as assets are loaded,
/// in the background.
///
/// Loading assets such as meshes will trigger the load of dependent assets
/// such as textures.
class AssetManager {
 public:
  /// @brief AssetManager constructor.
  /// @param[in] renderer A reference to the Renderer to use with the
  /// AssetManager.
  AssetManager(Renderer &renderer);

  /// @brief AssetManager destructor that purges all assets.
  ~AssetManager() { ClearAllAssets(); }

  /// @brief Returns a previously loaded shader object.
  ///
  /// @param basename The name of the shader.
  /// @return Returns the shader, or nullptr if not previously loaded.
  Shader *FindShader(const char *basename);

  /// @brief Loads and returns a shader object.
  ///
  /// Loads a shader if it hasn't been loaded already, by appending .glslv
  /// and .glslf to the basename, compiling and linking them.
  /// If this returns nullptr, the error can be found in Renderer::last_error().
  ///
  /// @param basename The name of the shader.
  /// @return Returns the loaded shader, or nullptr if there was an error.
  Shader *LoadShader(const char *basename);

  /// @brief Load a shader built by shader_pipeline.
  ///
  /// Loads a shader built by the shader_pipeline if it hasn't been loaded.
  /// already.  If this returns nullptr, the error can be found in
  /// Renderer::last_error().
  /// @param filename Name of the shader file to load.
  /// @return Returns the loaded shader, or nullptr if there was an error.
  Shader *LoadShaderDef(const char *filename);

  /// @brief Deletes the previously loaded shader.
  ///
  /// Deletes the shader and removes it from the material manager. Any
  /// subsequent requests for this shader through Load*() will cause them to be
  /// loaded anew.
  /// If its reference count was >1, it will be decreased instead of unloaded.
  ///
  /// @param filename The name of the shader to unload.
  void UnloadShader(const char *filename);

  /// @brief Returns a previously created texture.
  ///
  /// @param filename The name of the texture.
  /// @return Returns the texture, or nullptr if not previously loaded.
  Texture *FindTexture(const char *filename);

  /// @brief Queue loading a texture if it hasn't been loaded already.
  ///
  /// If async, queues a texture for loading if it hasn't been loaded already,
  /// otherwise loads it directly.
  /// Currently only supports TGA/WebP format files.
  /// If async, the returned texture isn't usable until TryFinalize() succeeds
  /// and the id is non-zero.
  ///
  /// @param filename The name of the texture to load.
  /// @param format The texture format, defaults to kFormatAuto.
  /// @param mipmaps If mipmaps should be used, defaults to true.
  /// @param async Whether to load the texture asynchronously.
  /// @return Returns an unloaded texture object. If not async, may also
  ///         return null to signal and error.
  Texture *LoadTexture(const char *filename, TextureFormat format = kFormatAuto,
                       bool mipmaps = true, bool async = true);

  /// @brief Start loading all previously queued textures.
  ///
  /// LoadTextures doesn't actually load anything, this will start the async
  /// loading of all files, and decompression.
  void StartLoadingTextures();

  /// @brief Check for the status of async loading textures.
  ///
  /// Call this repeatedly until it returns true, which signals all textures
  /// will have loaded, and turned into OpenGL textures.
  /// Textures with a 0 id will have failed to load.
  ///
  /// @return Returns true when all textures have been loaded.
  bool TryFinalize();

  /// @brief Deletes the previously loaded texture.
  ///
  /// Deletes the texture and removes it from the material manager. Any
  /// subsequent requests for this mesh through Load*() will cause them to be
  /// loaded anew.
  /// If its reference count was >1, it will be decreased instead of unloaded.
  ///
  /// @param filename The name of the texture to unload.
  void UnloadTexture(const char *filename);

  /// @brief Returns a previously loaded material.
  ///
  /// @param filename The name of the material.
  /// @return Returns the material, or nullptr if not previously loaded.
  Material *FindMaterial(const char *filename);

  /// @brief Loads and returns a material object.
  ///
  /// Loads a material, which is a compiled FlatBuffer file with
  /// root Material. This loads all resources contained there-in.
  /// If this returns nullptr, the error can be found in Renderer::last_error().
  ///
  /// @param filename The name of the material.
  /// @return Returns the loaded material, or nullptr if there was an error.
  Material *LoadMaterial(const char *filename);

  /// @brief Deletes the previously loaded material.
  ///
  /// Deletes all OpenGL textures contained in this material, and removes the
  /// textures and the material from material manager. Any subsequent requests
  /// for these textures through Load*() will cause them to be loaded anew.
  /// If its reference count was >1, it will be decreased instead of unloaded.
  ///
  /// @param filename The name of the material to unload.
  void UnloadMaterial(const char *filename);

  /// @brief Returns a previously loaded mesh.
  ///
  /// @param filename The name of the mesh.
  /// @return Returns the mesh, or nullptr if not previously loaded.
  Mesh *FindMesh(const char *filename);

  /// @brief Loads and returns a mesh object.
  ///
  /// Loads a mesh, which is a compiled FlatBuffer file with root Mesh.
  /// If this returns nullptr, the error can be found in Renderer::last_error().
  ///
  /// @param filename The name of the mesh.
  /// @return
  Mesh *LoadMesh(const char *filename);

  /// @brief Deletes the previously loaded mesh.
  ///
  /// Deletes the mesh and removes it from the material manager. Any subsequent
  /// requests for this mesh through Load*() will cause them to be loaded anew.
  /// If its reference count was >1, it will be decreased instead of unloaded.
  ///
  /// @param filename The name of the mesh to unload.
  void UnloadMesh(const char *filename);

  /// @brief Look up a previously loaded texture atlas.
  ///
  /// @param filename Name of the texture atlas file to look up.
  ///
  /// @return Pointer to the texture atlas if found, nullptr otherwise.
  TextureAtlas *FindTextureAtlas(const char *filename);

  /// @brief Loads a texture atlas.
  ///
  /// Loads a texture atlas, which is a compiled FlatBuffer file containing a
  /// texture path and subtexture rectangles.
  ///
  /// @return If this returns nullptr, the error can be found in
  /// Renderer::last_error().
  TextureAtlas *LoadTextureAtlas(const char *filename);

  /// @brief Delete a texture atlas and remove it from the asset manager.
  ///
  /// This will cause any Texture objects this atlas has issued to no
  /// longer refer to a valid texture.  Any subsequent requests for this texture
  /// atlas through Load*() will cause it to be loaded anew.
  /// If its reference count was >1, it will be decreased instead of unloaded.
  void UnloadTextureAtlas(const char *filename);

  /// @brief Look up a previously loaded file asset.
  ///
  /// @param filename Name of the file asset file to look up.
  ///
  /// @return Pointer to the file asset if found, nullptr otherwise.
  FileAsset *FindFileAsset(const char *filename);

  /// @brief Loads a file asset.
  ///
  /// @return nullptr on error.
  FileAsset *LoadFileAsset(const char *filename);

  /// @brief Delete a file asset and remove it from the asset manager.
  ///
  /// If its reference count was >1, it will be decreased instead of unloaded.
  void UnloadFileAsset(const char *filename);

  /// @brief Handy accessor for the renderer.
  ///
  /// @return Returns the renderer.
  Renderer &renderer() { return renderer_; }

  /// @brief Handy accessor for the renderer.
  ///
  /// @return Returns the renderer.
  const Renderer &renderer() const { return renderer_; }

  /// @brief Removes and destructs all assets held by the AssetManager.
  ///
  /// Will be called automatically by the destructor, but can also be called
  /// manually beforehand if necessary since destructing assets requires the
  /// OpenGL context to still be alive.
  void ClearAllAssets();

  /// @brief Set a scaling factor to apply when loading texture materials.
  ///
  /// By setting the scaling factor, an application save a memory footprint
  /// on low RAM devices.
  void SetTextureScale(const mathfu::vec2 &scale) { texture_scale_ = scale; }

 private:
  FPL_DISALLOW_COPY_AND_ASSIGN(AssetManager);

  // This implements the mechanism for each asset to be both loadable
  // sync or async.
  // It gets passed a blank asset that we take ownership of, and the map it
  // should go into if all succeeds.
  template<typename T> T *LoadOrQueue(T *asset,
                                      std::map<std::string, T *> &asset_map,
                                      bool async) {
    if (async) {
      loader_.QueueJob(asset);
    } else {
      bool ok = asset->LoadNow();
      if (!ok) {
        delete asset;
        return nullptr;
      }
    }
    asset_map[asset->filename()] = asset;
    return asset;
  }

  Renderer &renderer_;
  std::map<std::string, Shader *> shader_map_;
  std::map<std::string, Texture *> texture_map_;
  std::map<std::string, TextureAtlas *> texture_atlas_map_;
  std::map<std::string, Material *> material_map_;
  std::map<std::string, Mesh *> mesh_map_;
  std::map<std::string, FileAsset *> file_map_;
  AsyncLoader loader_;
  mathfu::vec2 texture_scale_;
};

/// @}
}  // namespace fplbase

#endif  // FPLBASE_ASSET_MANAGER_H
