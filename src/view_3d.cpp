#include "../include/view_3d.hpp"

#include <QColor>

View3D::View3D() {
    // Background color
    this->defaultFrameGraph()->setClearColor(QColor(0, 0, 0));
    // Camera
    _camera = this->camera();
    _camera->lens()->setPerspectiveProjection(
        45.0f, 16.0f / 9.0f, 0.1f, 1000.0f
    );
    _camera->setPosition(QVector3D(10.0f, 10.0f, 10.0f));
    _camera->setUpVector(QVector3D(0, 1, 0));
    _camera->setViewCenter(QVector3D(0, 0, 0));
    // napravi korijen
    this->_create_root_entity();
    // prikazi 3D prozor
    this->show();
    // napravi widget od ovog prozora
    this->_widget = QWidget::createWindowContainer(this);
    this->setFlags(Qt::FramelessWindowHint);
}

void View3D::_create_root_entity() {
    // Root entity
    root_entity = new Qt3DCore::QEntity();
    // napravi kontrolu kamere
    Qt3DExtras::QOrbitCameraController *camera_controller =
        new Qt3DExtras::QOrbitCameraController(root_entity);
    camera_controller->setLinearSpeed(200.0f);
    camera_controller->setLookSpeed(400.0f);
    camera_controller->setCamera(_camera);
    // svitlo
    Qt3DCore::QEntity *camera_light_entity = new Qt3DCore::QEntity(root_entity);
    Qt3DRender::QDirectionalLight *camera_light =
        new Qt3DRender::QDirectionalLight();
    camera_light->setWorldDirection(QVector3D(0.0f, -0.0f, -1.0f));
    camera_light->setColor(QColor::fromRgbF(1.0, 1.0, 1.0, 1.0));
    camera_light->setIntensity(1.50);
    camera_light_entity->addComponent(camera_light);
    Qt3DCore::QTransform *light_transform =
        new Qt3DCore::QTransform(camera_light_entity);
    light_transform->setTranslation(QVector3D(-20, 20, 20));
    camera_light_entity->addComponent(light_transform);
    // Materiali
    specular_white = new Qt3DExtras::QPhongMaterial(root_entity);
    specular_white->setDiffuse(QColor(255, 255, 255));
    specular_white->setSpecular(QColor(255, 255, 255));
    specular_white->setAmbient(QColor(255, 255, 255));

    specular_yellow = new Qt3DExtras::QPhongMaterial(root_entity);
    specular_yellow->setDiffuse(Qt::yellow);
    specular_yellow->setSpecular(Qt::yellow);
    specular_yellow->setAmbient(Qt::yellow);

    specular_green = new Qt3DExtras::QPhongMaterial(root_entity);
    specular_green->setDiffuse(Qt::green);
    specular_green->setSpecular(Qt::green);
    specular_green->setAmbient(Qt::green);

    specular_blue = new Qt3DExtras::QPhongMaterial(root_entity);
    specular_blue->setDiffuse(Qt::blue);
    specular_blue->setSpecular(Qt::blue);
    specular_blue->setAmbient(Qt::blue);

    specular_red = new Qt3DExtras::QPhongMaterial(root_entity);
    specular_red->setDiffuse(Qt::red);
    specular_red->setSpecular(Qt::red);
    specular_red->setAmbient(Qt::red);

    phong_red = new Qt3DExtras::QPhongMaterial(root_entity);
    phong_red->setDiffuse(QColor::fromRgbF(1.0, 0.0, 0.0, 1.0));

    phong_green = new Qt3DExtras::QPhongMaterial(root_entity);
    phong_green->setDiffuse(QColor::fromRgbF(0.0, 1.0, 0.0, 1.0));

    phong_blue = new Qt3DExtras::QPhongMaterial(root_entity);
    phong_blue->setDiffuse(QColor::fromRgbF(0.0, 0.0, 1.0, 1.0));

    phong_white = new Qt3DExtras::QPhongMaterial(root_entity);
    phong_white->setDiffuse(QColor::fromRgbF(1.0, 1.0, 1.0, 1.0));

    this->setRootEntity(root_entity);
}

void View3D::_add_triangles(
    const std::vector<YAAACD::Triangle> &triangles,
    Qt3DExtras::QPhongMaterial *material
) {
    std::vector<std::array<QVector3D, 3>> qtriangles;
    qInfo() << triangles.size();

    qtriangles.push_back({
        QVector3D(0, 0, 0),
        QVector3D(10, 10, 10),
        QVector3D(-10, -10, -10),
    });
    // std::transform(
    //     triangles.begin(),
    //     triangles.end(),
    //     std::back_inserter(qtriangles),
    //     [](const YAAACD::Triangle &triangle) {
    //         return std::array<QVector3D, 3>{
    //             QVector3D(triangle[0].x, triangle[0].y, triangle[0].z),
    //             QVector3D(triangle[1].x, triangle[1].y, triangle[1].z),
    //             QVector3D(triangle[2].x, triangle[2].y, triangle[2].z)
    //         };
    //     }
    // );

    Qt3DCore::QEntity *triangles_entity = new Qt3DCore::QEntity(root_entity);
    Qt3DRender::QGeometryRenderer *triangles_renderer =
        new Qt3DRender::QGeometryRenderer;
    Qt3DCore::QGeometry *triangles_geometry =
        new Qt3DCore::QGeometry(triangles_renderer);
    // qt3d vertex i index bufferi
    Qt3DCore::QBuffer *triangles_vertex_data_buffer =
        new Qt3DCore::QBuffer(triangles_geometry);
    Qt3DCore::QBuffer *triangles_index_data_buffer =
        new Qt3DCore::QBuffer(triangles_geometry);

    QVector3D triangle_normal;
    QVector<QVector3D> triangles_vertices;
    for (auto triangle : qtriangles) {
        triangle_normal =
            QVector3D::normal(triangle[0], triangle[1], triangle[2]);
        triangles_vertices << triangle[0] << triangle_normal;
        triangles_vertices << triangle[1] << triangle_normal;
        triangles_vertices << triangle[2] << triangle_normal;
    }
    QByteArray triangles_vertex_buffer;
    triangles_vertex_buffer.resize(
        int(qtriangles.size()) * 3 * (3 + 3) * sizeof(float)
    );
    float *triangles_raw_vertices =
        reinterpret_cast<float *>(triangles_vertex_buffer.data());
    int idx = 0;
    for (const QVector3D &vertex : triangles_vertices) {
        triangles_raw_vertices[idx++] = vertex.x();
        triangles_raw_vertices[idx++] = vertex.y();
        triangles_raw_vertices[idx++] = vertex.z();
    }
    // stvaram index buffer
    QByteArray triangles_index_buffer_data;
    // broj trokuta * 3 vrha
    triangles_index_buffer_data.resize(
        int(qtriangles.size()) * 3 * sizeof(ushort)
    );
    ushort *triangles_raw_indices =
        reinterpret_cast<ushort *>(triangles_index_buffer_data.data());
    for (ulong i = 0; i < qtriangles.size(); ++i) {
        triangles_raw_indices[i * 3 + 0] = i * 3 + 0;
        triangles_raw_indices[i * 3 + 1] = i * 3 + 1;
        triangles_raw_indices[i * 3 + 2] = i * 3 + 2;
    }

    // postavi buffere
    triangles_vertex_data_buffer->setData(triangles_vertex_buffer);
    triangles_index_data_buffer->setData(triangles_index_buffer_data);

    // Attributes za Qt3D
    Qt3DCore::QAttribute *triangles_positions = new Qt3DCore::QAttribute();
    triangles_positions->setAttributeType(Qt3DCore::QAttribute::VertexAttribute
    );
    triangles_positions->setBuffer(triangles_vertex_data_buffer);
    triangles_positions->setVertexBaseType(Qt3DCore::QAttribute::Float);
    triangles_positions->setVertexSize(3);
    triangles_positions->setByteOffset(0);
    triangles_positions->setByteStride(2 * 3 * sizeof(float));
    triangles_positions->setCount(int(qtriangles.size()) * 3);
    triangles_positions->setName(
        Qt3DCore::QAttribute::defaultPositionAttributeName()
    );
    // normale
    Qt3DCore::QAttribute *triangles_normals = new Qt3DCore::QAttribute();
    triangles_normals->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    triangles_normals->setBuffer(triangles_vertex_data_buffer);
    triangles_normals->setVertexBaseType(Qt3DCore::QAttribute::Float);
    triangles_normals->setVertexSize(3);
    triangles_normals->setByteOffset(3 * sizeof(float));
    triangles_normals->setByteStride(2 * 3 * sizeof(float));
    triangles_normals->setCount(int(qtriangles.size()) * 3);
    triangles_normals->setName(Qt3DCore::QAttribute::defaultNormalAttributeName(
    ));
    // indeksi lica
    Qt3DCore::QAttribute *triangles_indices = new Qt3DCore::QAttribute();
    triangles_indices->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    triangles_indices->setBuffer(triangles_index_data_buffer);
    triangles_indices->setVertexBaseType(Qt3DCore::QAttribute::UnsignedShort);
    triangles_indices->setVertexSize(1);
    triangles_indices->setByteOffset(0);
    triangles_indices->setByteStride(0);
    triangles_indices->setCount(int(qtriangles.size()) * 3);
    // dodaj atribute
    triangles_geometry->addAttribute(triangles_positions);
    triangles_geometry->addAttribute(triangles_normals);
    triangles_geometry->addAttribute(triangles_indices);
    // podešavam qt3d mesh objekt
    triangles_renderer->setInstanceCount(1);
    triangles_renderer->setFirstVertex(0);
    triangles_renderer->setFirstInstance(0);
    triangles_renderer->setPrimitiveType(
        Qt3DRender::QGeometryRenderer::Triangles
    );
    triangles_renderer->setGeometry(triangles_geometry);
    // broj trokuta * 3 tocke
    triangles_renderer->setVertexCount(int(qtriangles.size()) * 3);
    // dodaj renderer u entitet
    triangles_entity->addComponent(triangles_renderer);
    // dodaj materijal u entitet
    triangles_entity->addComponent(material);
}

void View3D::add_lines(
    const std::vector<std::array<YAAACD::Vertex, 2>> &lines,
    Qt3DExtras::QPhongMaterial *material
) {
    // stvaram entitet, renderer i geometriju
    Qt3DCore::QEntity *lines_entity = new Qt3DCore::QEntity(this->root_entity);
    Qt3DRender::QGeometryRenderer *lines_renderer =
        new Qt3DRender::QGeometryRenderer(root_entity);
    Qt3DCore::QGeometry *lines_geometry = new Qt3DCore::QGeometry(root_entity);
    // qt3d vertex i index baferi
    Qt3DCore::QBuffer *lines_vertex_data_buffer =
        new Qt3DCore::QBuffer(lines_geometry);
    Qt3DCore::QBuffer *lines_index_data_buffer =
        new Qt3DCore::QBuffer(lines_geometry);

    QVector<QVector3D> lines_vertices;

    std::for_each(
        lines.begin(),
        lines.end(),
        [&lines_vertices](const std::array<YAAACD::Vertex, 2> &line) {
            lines_vertices << QVector3D(line[0].x, line[0].y, line[0].z)
                           << QVector3D(line[1].x, line[1].y, line[1].z);
        }
    );

    QByteArray line_vertices_buffer;
    line_vertices_buffer.resize(lines.size() * 2 * 3 * sizeof(float));
    float *lines_raw_vertices =
        reinterpret_cast<float *>(line_vertices_buffer.data());
    int idx = 0;
    for (const QVector3D &vertex : lines_vertices) {
        lines_raw_vertices[idx++] = vertex.x();
        lines_raw_vertices[idx++] = vertex.y();
        lines_raw_vertices[idx++] = vertex.z();
    }
    // connectivity between vertices
    QByteArray linesIndexBufferData;
    linesIndexBufferData.resize(
        lines.size() * sizeof(unsigned int)
    );  // start to end
    unsigned int *linesRawIndexArray =
        reinterpret_cast<unsigned int *>(linesIndexBufferData.data());
    for (ulong i = 0; i < lines.size(); i++) {
        *linesRawIndexArray++ = i;
    }
    // postavljam buffere
    lines_vertex_data_buffer->setData(line_vertices_buffer);
    lines_index_data_buffer->setData(linesIndexBufferData);
    // stvaram atribute koordinata
    Qt3DCore::QAttribute *lines_positions =
        new Qt3DCore::QAttribute(lines_geometry);
    lines_positions->setName(Qt3DCore::QAttribute::defaultPositionAttributeName(
    ));
    lines_positions->setVertexBaseType(Qt3DCore::QAttribute::Float);
    lines_positions->setVertexSize(3);
    lines_positions->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    lines_positions->setBuffer(lines_vertex_data_buffer);
    lines_positions->setByteStride(3 * sizeof(float));
    lines_positions->setCount(lines.size());
    // stvaram atribute indeksa linija
    Qt3DCore::QAttribute *lines_indices =
        new Qt3DCore::QAttribute(lines_geometry);
    lines_indices->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    lines_indices->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    lines_indices->setBuffer(lines_index_data_buffer);
    lines_indices->setCount(lines.size());
    // dodajem atribute u geometriju
    lines_geometry->addAttribute(lines_positions
    );  // We add the vertices in the lines_geometry
    lines_geometry->addAttribute(lines_indices
    );  // We add the indices linking the points in the lines_geometry
        // dodajem geometriju
    lines_renderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    lines_renderer->setGeometry(lines_geometry);
    // dodajem objekt
    lines_entity->addComponent(lines_renderer);
    lines_entity->addComponent(material);
}

void View3D::add_triangles(
    const std::vector<YAAACD::Triangle> &triangles,
    Qt3DExtras::QPhongMaterial *_material
) {
    auto material = this->phong_red;
    size_t step = 5000;

    if (triangles.size() > step) {
        qInfo() << "Triangles: " << triangles.size();
        size_t passed = 0;

        do {
            auto start = triangles.begin() + passed;
            size_t distance = std::distance(start, triangles.end());
            auto end = distance > step ? start + step : triangles.end();

            this->_add_triangles(
                std::vector<YAAACD::Triangle>(start, end), material
            );
            passed += step;
        } while (passed <= triangles.size());

        return;
    }

    this->_add_triangles(triangles, material);
}

void View3D::add_obj_file(std::string filename) {
    Qt3DCore::QEntity *mesh_entity = new Qt3DCore::QEntity(this->root_entity);
    Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh(this->root_entity);
    mesh->setSource(QUrl::fromLocalFile(filename.c_str()));

    mesh_entity->addComponent(mesh);
    mesh_entity->addComponent(this->phong_red);
}

void View3D::draw_bounding_box(const YAAACD::BoundingBox &box) {
    auto corners = box.corners();
    std::vector<std::array<YAAACD::Vertex, 2>> pairs{
        {corners[0], corners[1]},
        {corners[1], corners[5]},

        {corners[5], corners[4]},
        {corners[4], corners[0]},

        {corners[2], corners[3]},
        {corners[3], corners[7]},

        {corners[7], corners[6]},
        {corners[6], corners[2]},

        {corners[0], corners[2]},
        {corners[1], corners[3]},

        {corners[5], corners[7]},
        {corners[4], corners[6]},
    };

    this->add_lines(pairs, this->specular_blue);
}
