#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();

private:
    Ui::MainWindow *ui;
    QLabel *resultLabel;

    bool processFile(const QString& inputFilePath, const QString& outputFilePath, const QString& logFilePath);
    void logMessage(QFile& logFile, const QString& message);
};

#endif // MAINWINDOW_H
