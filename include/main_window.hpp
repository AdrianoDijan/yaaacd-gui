#pragma once

#include <QFile>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

#include "./view_3d.hpp"

using namespace std;

class LogBox : public QPlainTextEdit {
 public:
    void add_message(const QString& text);
    LogBox& operator<<(string text);
    LogBox& operator<<(int number);
};

class MainWindow : public QMainWindow {
 public:
    MainWindow();

    QWidget* top_widget;
    LogBox* log_box;
    QVBoxLayout* layout;
    View3D* view_3d;
};
