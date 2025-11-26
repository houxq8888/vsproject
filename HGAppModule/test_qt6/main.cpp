#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QWidget window;
    window.setWindowTitle("Qt6 Test - HGAppModule");
    window.resize(400, 200);
    
    QVBoxLayout *layout = new QVBoxLayout(&window);
    QLabel *label = new QLabel("HGAppModule Qt6 Windows Build Test - Success!");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    
    window.show();
    
    return app.exec();
}