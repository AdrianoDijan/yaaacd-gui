#include <QApplication>
#include <QUrl>
#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <chrono>
#include <yaaacd/hashmap.hpp>
#include <yaaacd/octree.hpp>

#include "../include/main_window.hpp"
#include "../include/objfile.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow main_window;
    View3D *view_3d = main_window.view_3d;
    LogBox *log_box = main_window.log_box;

    std::vector<std::string> args;

    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    main_window.showMaximized();

    auto start_time = chrono::high_resolution_clock::now();
    *(log_box) << "Parsing first file // ";
    auto triangles1 = parse_obj_file(argv[1]);
    int duration = chrono::duration_cast<chrono::milliseconds>(
                       chrono::high_resolution_clock::now() - start_time
    )
                       .count();
    *(log_box) << duration << " ms\n";

    start_time = chrono::high_resolution_clock::now();
    *(log_box) << "Parsing second file // ";
    auto triangles2 = parse_obj_file(argv[2]);
    duration = chrono::duration_cast<chrono::milliseconds>(
                   chrono::high_resolution_clock::now() - start_time
    )
                   .count();
    *(log_box) << duration << " ms\n";

    start_time = chrono::high_resolution_clock::now();
    *(log_box) << "Drawing first structure // ";
    view_3d->add_obj_file(argv[1]);
    duration = chrono::duration_cast<chrono::milliseconds>(
                   chrono::high_resolution_clock::now() - start_time
    )
                   .count();
    *(log_box) << duration << " ms\n";

    start_time = chrono::high_resolution_clock::now();
    *(log_box) << "Drawing second structure // ";
    view_3d->add_obj_file(argv[2]);
    duration = chrono::duration_cast<chrono::milliseconds>(
                   chrono::high_resolution_clock::now() - start_time
    )
                   .count();
    *(log_box) << duration << " ms\n";

    if (std::find(args.begin(), args.end(), "--bruteforce") != args.end()) {
        start_time = chrono::high_resolution_clock::now();
        *(log_box) << "Checking for collisions [Bruteforce] // ";
        bool bruteforce_collides =
            YAAACD::bruteforce_collides(triangles1, triangles2);
        duration = chrono::duration_cast<chrono::milliseconds>(
                       chrono::high_resolution_clock::now() - start_time
        )
                       .count();
        *(log_box) << duration << " ms\n";
        *(log_box) << "Collision detected: "
                   << (bruteforce_collides ? "true" : "false") << "\n";
    }

    start_time = chrono::high_resolution_clock::now();
    *(log_box) << "Building first Octree structure // ";
    auto tree1 = YAAACD::Octree(triangles1);
    duration = chrono::duration_cast<chrono::milliseconds>(
                   chrono::high_resolution_clock::now() - start_time
    )
                   .count();
    *(log_box) << duration << " ms\n";

    start_time = chrono::high_resolution_clock::now();
    *(log_box) << "Building second Octree structure // ";
    auto tree2 = YAAACD::Octree(triangles2);
    duration = chrono::duration_cast<chrono::milliseconds>(
                   chrono::high_resolution_clock::now() - start_time
    )
                   .count();
    *(log_box) << duration << " ms\n";

    start_time = chrono::high_resolution_clock::now();
    *(log_box) << "Checking for collisions [Octree] // ";
    bool octree_collides = tree1.collides(&tree2);
    duration = chrono::duration_cast<chrono::milliseconds>(
                   chrono::high_resolution_clock::now() - start_time
    )
                   .count();
    *(log_box) << duration << " ms\n";
    *(log_box) << "Collision detected: " << (octree_collides ? "true" : "false")
               << "\n";

    start_time = chrono::high_resolution_clock::now();
    *(log_box) << "Building hashmap for the first object // ";
    auto hashmap = YAAACD::SpatialHashMap(triangles1, 3);
    duration = chrono::duration_cast<chrono::milliseconds>(
                   chrono::high_resolution_clock::now() - start_time
    )
                   .count();
    *(log_box) << duration << " ms\n";

    start_time = chrono::high_resolution_clock::now();
    *(log_box) << "Checking for collisions [SpatialHashMap] // ";
    bool hashmap_collides = hashmap.collides(triangles2);
    duration = chrono::duration_cast<chrono::milliseconds>(
                   chrono::high_resolution_clock::now() - start_time
    )
                   .count();
    *(log_box) << duration << " ms\n";
    *(log_box) << "Collision detected: "
               << (hashmap_collides ? "true" : "false") << "\n";

    return app.exec();
}
