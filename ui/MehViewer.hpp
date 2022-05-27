#pragma once

#include <meh/logic.hpp>
#include <pmp/algorithms/SurfaceSmoothing.h>
#include <pmp/visualization/MeshViewer.h>

using namespace pmp;

class MehViewer : public pmp::MeshViewer {
public:
  //! constructor
  MehViewer(const char *title, int width, int height);

  virtual void load_mesh(const char *filename) override;
  /* void unload_mesh(int idx); */

  void draw(const std::string &draw_mode) override;

  void toggle_mesh_vis(int idx);

  void add_mesh_texture(int idx, char *filename);

protected:
  //! this function handles mouse button presses
  void mouse(int button, int action, int mods) override;

  //! this function handles keyboard events
  void keyboard(int key, int code, int action, int mod) override;

  virtual void process_imgui() override;

  virtual void process_imgui_menubar() override;

  SurfaceMeshGL meshes_[10];
  std::string names_[10];
  std::string texnames_[10];
  bool vis_[10];
  bool tex_[10];
  int meshCount_;
  int cMesh;
  int *cMesh_;
};
