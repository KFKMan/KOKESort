#include <QApplication>
#include <QPushButton>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QPushButton button("Click me");
    button.resize(200, 100);

    QObject::connect(&button, &QPushButton::clicked, []() {
        QMessageBox::information(nullptr, "Hii", "You are clicked the button yeey");
    });

    button.show();
    return app.exec();
}
