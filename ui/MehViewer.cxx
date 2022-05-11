#include "MehViewer.hpp"

#include "ImGuiFileDialog.h"
#include <imgui.h>
#include <iostream>

using namespace meh;

MehViewer::MehViewer(const char *title, int width, int height)
    : MeshViewer(title, width, height) {
  clear_help_items();
  clear_draw_modes();
  add_draw_mode("Smooth Shading");
  add_draw_mode("Hidden Line");
  set_draw_mode("Smooth Shading");
  set_draw_mode("Points");
  meshCount_ = 0;
}

void MehViewer::keyboard(int key, int scancode, int action, int mods) {
  if (action != GLFW_PRESS && action != GLFW_REPEAT)
    return;

  switch (key) {
  case '?': {
    break;
  }
  default: {
    MeshViewer::keyboard(key, scancode, action, mods);
    break;
  }
  }
}

void MehViewer::load_mesh(const char *filename) {
  std::cout << "Loading " << filename << std::endl;
  if (meshCount_ >= 10) {
    return;
  }

  try {
    meshes_[meshCount_].read(filename);
    names_[meshCount_] = filename;
    vis_[meshCount_] = true;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    throw;
  }

  BoundingBox bb = meshes_[meshCount_].bounds();
  set_scene((vec3)bb.center(), 0.5 * bb.size());

  meshCount_++;
}

void MehViewer::draw(const std::string &drawMode) {
  for (int i = 0; i < meshCount_; i++) {
    if (vis_[i]) {
      meshes_[i].draw(projection_matrix_, modelview_matrix_, drawMode);
    }
  }
}

void MehViewer::process_imgui() {
  if (ImGuiFileDialog::Instance()->Display("ChooseNewMesh")) {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      // action
      std::cout << filePathName << " " << filePath << std::endl;
      MehViewer::load_mesh(&filePathName[0]);
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }

  if (ImGuiFileDialog::Instance()->Display("SaveNew3MF")) {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      // action
      std::cout << filePathName << " " << filePath << std::endl;
      char *inputs[meshCount_];
      for (int i = 0; i < meshCount_; i++) {
              inputs[i] = &(names_[i])[0];
      }

      meh::exportModelListTo3MF(inputs, &filePathName[0], meshCount_);
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }

  for (int i = 0; i < meshCount_; i++) {
    ImGui::BulletText(&(names_[i])[0]);
    ImGui::BulletText("%d vertices", (int)meshes_[i].n_vertices());
    ImGui::BulletText("%d edges", (int)meshes_[i].n_edges());
    ImGui::BulletText("%d faces", (int)meshes_[i].n_faces());
    ImGui::Separator();
  }
}

void MehViewer::process_imgui_menubar() {
  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("Open..", "Ctrl+O")) {
      ImGuiFileDialog::Instance()->OpenDialog("ChooseNewMesh", "Choose File",
                                              ".obj,.stl,.off", ".");
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
      ImGuiFileDialog::Instance()->OpenDialog(
          "SaveNew3MF", "Choose a save Location", ".3mf", ".");
    }
    ImGui::EndMenu();
  }
  ImGui::EndMenuBar();
}

void MehViewer::mouse(int button, int action, int mods) {
  MeshViewer::mouse(button, action, mods);
}
