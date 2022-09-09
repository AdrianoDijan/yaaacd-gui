#include "../include/main_window.hpp"

MainWindow::MainWindow() {
    view_3d = new View3D;
    layout = new QVBoxLayout;
    log_box = new LogBox;

    QPalette log_box_palette = log_box->palette();
    log_box_palette.setColor(QPalette::Base, Qt::black);
    log_box_palette.setColor(QPalette::Text, Qt::green);
    log_box->setPalette(log_box_palette);
    log_box->setReadOnly(true);

    layout->addWidget(view_3d->_widget);
    view_3d->_widget->setMinimumHeight(500);
    layout->setStretch(0, 2);

    layout->addWidget(log_box);

    top_widget = new QWidget;
    top_widget->setLayout(layout);
    setCentralWidget(top_widget);
}

void LogBox::add_message(const QString& text) {
    this->moveCursor(QTextCursor::End);
    this->insertPlainText(text);
    this->moveCursor(QTextCursor::End);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()
    );  // Scrolls to the bottom
}

LogBox& LogBox::operator<<(std::string text) {
    std::cout << text.c_str() << std::flush;
    this->add_message(QString::fromStdString(text));
    return *this;
}

LogBox& LogBox::operator<<(int number) {
    std::cout << number << std::flush;
    this->add_message(QString::fromStdString(to_string(number)));
    return *this;
}
