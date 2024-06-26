#include "MehViewer.hpp"

#include "ImGuiFileDialog.h"
#include "pmp/algorithms/utilities.h"
#include "pmp/bounding_box.h"
#include <imgui.h>
#include <iostream>

using namespace meh;

MehViewer::MehViewer(const char *title, int width, int height)
    : MeshViewer(title, width, height) {
  clear_help_items();
  clear_draw_modes();
  add_draw_mode("Smooth Shading");
  add_draw_mode("Hidden Line");
  add_draw_mode("Points");
  add_draw_mode("Texture");
  set_draw_mode("Smooth Shading");
  meshCount_ = 0;
  cMesh = 0;
  cMesh_ = &cMesh;
  config.path = ".";
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
    read_pmp(meshes_[meshCount_], (filename));
    names_[meshCount_] = filename;
    vis_[meshCount_] = true;
    tex_[meshCount_] = false;
    texnames_[meshCount_] = "";

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    throw;
  }

  MehViewer::focus_mesh(meshCount_);

  meshCount_++;
}

void MehViewer::focus_mesh(int idx) {
  BoundingBox bb = bounds(meshes_[idx]);
  set_scene((vec3)bb.center(), 0.5 * bb.size());
}

void MehViewer::draw(const std::string &drawMode) {
  for (int i = 0; i < meshCount_; i++) {
    if (vis_[i]) {
      Renderer render(meshes_[i]);
      if (tex_[i]) {
        render.load_texture(texnames_[i].c_str());
      }
      render.draw(projection_matrix_, modelview_matrix_, drawMode);
    }
  }
}

void MehViewer::process_imgui() {
  float display_w = 500;
  float display_h = 500;
  ImVec2 maxSize = ImVec2((float)display_w * 1.5f, (float)display_h * 1.5f);
  ImVec2 minSize = ImVec2((float)display_w, (float)display_h);
  if (ImGuiFileDialog::Instance()->Display(
          "ChooseNewMesh", ImGuiWindowFlags_NoCollapse, minSize, maxSize)) {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      config.path = filePath + "/";
      // action
      std::cout << filePathName << " " << filePath << std::endl;
      MehViewer::load_mesh(&filePathName[0]);
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }

  if (ImGuiFileDialog::Instance()->Display(
          "SaveNew3MF", ImGuiWindowFlags_NoCollapse, minSize, maxSize)) {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      config.path = filePath + "/";
      // action
      std::cout << filePathName << " " << filePath << std::endl;
      char *inputs[meshCount_];
      char *texInputs[meshCount_];
      char none[] = "None";
      for (int i = 0; i < meshCount_; i++) {
        inputs[i] = &(names_[i])[0];
        if (tex_[i]) {
          texInputs[i] = &(texnames_[i])[0];
        } else {
          texInputs[i] = none;
        }
      }

      meh::exportModelListTo3MF(inputs, texInputs, &filePathName[0],
                                meshCount_);
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }

  if (ImGuiFileDialog::Instance()->Display(
          "ChooseNewTexture", ImGuiWindowFlags_NoCollapse, minSize, maxSize)) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      config.path = filePath + "/";

      std::cout << filePathName << " " << std::endl;
      MehViewer::add_mesh_texture(cMesh, &filePathName[0]);
    }

    ImGuiFileDialog::Instance()->Close();
  }

  for (int i = 0; i < meshCount_; i++) {
    ImGui::Text("Mesh %d", i);
    ImGui::Text("%s", &(names_[i])[0]);
    if (tex_[i]) {
      ImGui::Text("%s", &(texnames_[i])[0]);
    }
    ImGui::BulletText("%d vertices", (int)meshes_[i].n_vertices());
    ImGui::BulletText("%d edges", (int)meshes_[i].n_edges());
    ImGui::BulletText("%d faces", (int)meshes_[i].n_faces());
    ImGui::Separator();
  }
  if (meshCount_ > 0) {
    if (meshCount_ > 1) {
      ImGui::SliderInt("", cMesh_, 0, meshCount_ - 1, "Mesh %d");
    }
    ImGui::Text("Mesh Options");
    if (ImGui::Button("Toggle Visibility"))
      MehViewer::toggle_mesh_vis(cMesh);
    if (ImGui::Button("Add Texture")) {
      ImGuiFileDialog::Instance()->OpenDialog(
          "ChooseNewTexture", "Choose Texture", ".png,.jpg,.jpeg", config);
    }
    if (ImGui::Button("Remove Mesh")) {
      MehViewer::remove_mesh(cMesh);
    }
    if (ImGui::Button("Focus Mesh")) {
      MehViewer::focus_mesh(cMesh);
    }
  }

  process_imgui_menubar();
}

void MehViewer::process_imgui_menubar() {
  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("Open..", "Ctrl+O")) {
      ImGuiFileDialog::Instance()->OpenDialog("ChooseNewMesh", "Choose File",
                                              ".obj,.stl,.off", config);
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
      ImGuiFileDialog::Instance()->OpenDialog(
          "SaveNew3MF", "Choose a save Location", ".3mf", config);
    }
    ImGui::EndMenu();
  }
  ImGui::EndMenuBar();
}

void MehViewer::mouse(int button, int action, int mods) {
  MeshViewer::mouse(button, action, mods);
}

void MehViewer::toggle_mesh_vis(int idx) {
  if (vis_[idx])
    vis_[idx] = false;
  else
    vis_[idx] = true;
}

void MehViewer::add_mesh_texture(int idx, char *filename) {
  texnames_[idx] = filename;
  tex_[idx] = true;

  set_draw_mode("Texture");
}

void MehViewer::remove_mesh(int idx) {
  meshes_[idx].clear();
  meshes_[idx].free_memory();

  for (int i = idx; i < meshCount_ - 1; i++) {
    if (i < meshCount_) {
      meshes_[i] = meshes_[i + 1];
      names_[i] = names_[i + 1];
      vis_[i] = vis_[i + 1];
      tex_[i] = tex_[i + 1];
      texnames_[i] = texnames_[i + 1];
    }
  }
  meshCount_--;
  MehViewer::focus_mesh(0);
}
