#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем виджет внутри mainWindow
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *label = new QLabel("Нажмите кнопку для подсчета слов", this);
    layout->addWidget(label);

    QPushButton *button = new QPushButton("Запустить подсчет", this);
    layout->addWidget(button);

    resultLabel = new QLabel("", this);
    layout->addWidget(resultLabel);

    setCentralWidget(centralWidget);

    // Соединение кнопки с обработчиком
    connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonClicked() {
    QString inputFilePath = "WordsList.txt";
    QString outputFilePath = "ResultPairs.txt";
    QString logFilePath = "Log.txt";

    // Вывод результата
    if (processFile(inputFilePath, outputFilePath, logFilePath)) {
        resultLabel->setText("Подсчет завершен. Результаты сохранены в ResultPairs.txt.");
    } else {
        resultLabel->setText("Произошла ошибка. Подробности в Log.txt.");
    }
}

bool MainWindow::processFile(const QString& inputFilePath, const QString& outputFilePath, const QString& logFilePath) {
    QFile inputFile(inputFilePath);
    QFile outputFile(outputFilePath);
    QFile logFile(logFilePath);

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logMessage(logFile, "Error: Could not open input file.");
        return false;
    }

    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logMessage(logFile, "Error: Could not open output file.");
        return false;
    }

    QTextStream in(&inputFile);
    QTextStream out(&outputFile);
    std::map<QString, int> wordCount;

    // Подсчет слов
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList words = line.split(QRegExp("\\W+"), Qt::SkipEmptyParts);
        for (QString& word : words) {
            word = word.toLower();
            wordCount[word]++;
        }
    }

    inputFile.close();

    // Сортировка результата
    std::vector<std::pair<QString, int>> sortedWords(wordCount.begin(), wordCount.end());
    std::sort(sortedWords.begin(), sortedWords.end(),
              [](const std::pair<QString, int>& a, const std::pair<QString, int>& b) {
                  if (a.second == b.second) return a.first < b.first;
                  return a.second > b.second;
              });

    for (const auto& pair : sortedWords) {
        out << pair.first << " - " << pair.second << "\n";
    }

    outputFile.close();
    logMessage(logFile, "Success: Result file generated.");
    return true;
}

void MainWindow::logMessage(QFile& logFile, const QString& message) {
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream logStream(&logFile);
        logStream << message << "\n";
        logFile.close();
    }
}
