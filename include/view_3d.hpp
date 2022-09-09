#pragma once

#include <QWidget>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DRender/QDirectionalLight>
#include <array>
#include <vector>
#include <yaaacd/common.hpp>

class View3D : public Qt3DExtras::Qt3DWindow {
 private:
    Qt3DExtras::QPhongMaterial* specular_white;
    Qt3DExtras::QPhongMaterial* specular_yellow;
    Qt3DExtras::QPhongMaterial* specular_green;
    Qt3DExtras::QPhongMaterial* specular_blue;
    Qt3DExtras::QPhongMaterial* specular_red;
    Qt3DExtras::QPhongMaterial* phong_red;
    Qt3DExtras::QPhongMaterial* phong_green;
    Qt3DExtras::QPhongMaterial* phong_blue;
    Qt3DExtras::QPhongMaterial* phong_white;
    void _add_triangles(
        const std::vector<YAAACD::Triangle>& triangles,
        Qt3DExtras::QPhongMaterial* material = nullptr
    );

 public:
    View3D();

    void add_triangles(
        const std::vector<YAAACD::Triangle>& triangles,
        Qt3DExtras::QPhongMaterial* material = nullptr
    );
    void add_obj_file(std::string filename);
    void add_lines(
        const std::vector<std::array<YAAACD::Vertex, 2>>& lines,
        Qt3DExtras::QPhongMaterial* material
    );
    void _create_root_entity();
    QWidget* widget() {
        return this->_widget;
    }
    void draw_bounding_box(const YAAACD::BoundingBox& box);

    QWidget* _widget;
    Qt3DCore::QEntity* root_entity;
    Qt3DRender::QCamera* _camera;
};