#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Окно Множественный выбор
void MainWindow::on_pushButton_7_clicked()
{
    MultipleChoiseWindow *newMPWindow = new MultipleChoiseWindow();
    newMPWindow->exec();
    updateListWidget();
    delete newMPWindow;
}

//Окно Пропущенное слово
void MainWindow::on_pushButton_6_clicked()
{
    MissedWordDialog *newMWWindow = new MissedWordDialog();
    newMWWindow->exec();
    updateListWidget();
    delete newMWWindow;
}

//Окно Короткий ответ
void MainWindow::on_pushButton_2_clicked()
{
    ShortAnswerDialog *newSAWindow = new ShortAnswerDialog();
    newSAWindow->exec();
    updateListWidget();
    delete newSAWindow;
}

//Окно Верно-неверно
void MainWindow::on_pushButton_5_clicked()
{
    TrueFalseDialog *newTFWindow = new TrueFalseDialog();
    newTFWindow->exec();
    updateListWidget();
    delete newTFWindow;
}

//Окно выбор соотвествия
void MainWindow::on_pushButton_4_clicked()
{
    MatchingSelectionDialog *newMSWindow = new MatchingSelectionDialog();
    newMSWindow->exec();
    updateListWidget();
    delete newMSWindow;
}

//Окно числовой ответ
void MainWindow::on_pushButton_3_clicked()
{
    NumericAnswerDialog *newNAWindow = new NumericAnswerDialog();
    newNAWindow->exec();
    updateListWidget();
    delete newNAWindow;
}

//Окно Эссе
void MainWindow::on_pushButton_clicked()
{
    DialogEssay *newEWindow = new DialogEssay();
    newEWindow->exec();
    updateListWidget();
    delete newEWindow;
}
//Открыть файл
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open the file", "Gift File", tr("Gift (*.gift *.GIFT *.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    modelClear();
    ControllerApp *newApp = new ControllerApp();
    addItemsToView(newApp->parseFile(file));
    setWindowTitle(fileName);
    QTextStream in(&file);
    file.close();
    delete newApp;
}

//Обновить QListWidget
void MainWindow::updateListWidget() {
    ControllerApp *newAppController = new ControllerApp();
    ui->listWidget->clear();
    addItemsToView(newAppController->requestModelData());
    delete newAppController;
}

void MainWindow::addItemsToView(QStringList strList) {
    for (int i = 0; i < strList.count(); i++){
        QListWidgetItem *item = new QListWidgetItem(strList[i]);
        ui->listWidget->addItem(item);
    }

}

//Редактирование вопросов
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item){
    ControllerApp *newController = new ControllerApp();
    int index = ui->listWidget->row(item);
    QString qType = newController->requestQuestType(index);
    QStringList data = newController->getQuestData(index);
    if (qType == "Multiple Choise") {
        MultipleChoiseWindow *newMPWindow = new MultipleChoiseWindow();
        newMPWindow->editable = true;
        newMPWindow->index = index;
        newMPWindow->setUpUi(data);
        newMPWindow->exec();
        delete newMPWindow;
    }
    if (qType == "Short Answer") {
        ShortAnswerDialog *newSAWindow = new ShortAnswerDialog();
        newSAWindow->editable = true;
        newSAWindow->index = index;
        newSAWindow->setUpUI(data);
        newSAWindow->exec();
        delete newSAWindow;
    }
    if (qType == "True False") {
        TrueFalseDialog *newTFWindow = new TrueFalseDialog();
        newTFWindow->editable = true;
        newTFWindow->index = index;
        newTFWindow->setUpUI(data);
        newTFWindow->exec();
        delete newTFWindow;
    }
    if (qType == "Matching Selection") {
        MatchingSelectionDialog *newMSWindow = new MatchingSelectionDialog();
        newMSWindow->editable = true;
        newMSWindow->index = index;
        newMSWindow->setUpUI(data);
        newMSWindow->exec();
        delete newMSWindow;
    }
    if (qType == "Numeric Answer") {
        NumericAnswerDialog *newNAWindow = new NumericAnswerDialog();
        newNAWindow->editable = true;
        newNAWindow ->index = index;
        newNAWindow->setUpUI(data);
        newNAWindow->exec();
        delete newNAWindow;
    }
    if (qType == "Essay") {
        DialogEssay *newEWindow = new DialogEssay();
        newEWindow->editable = true;
        newEWindow->index = index;
        newEWindow->setUpUI(data);
        newEWindow->exec();
        delete newEWindow;
    }
    delete newController;
    updateListWidget();
}

//Сохранить как
void MainWindow::on_action_triggered()
{
    //QString fileName = QFileDialog::getSaveFileName(this,"Save as", "Test", tr("Gift (*.gift *.GIFT *.txt)"));
    QString fileName = QFileDialog::getSaveFileName(this,"Save as", "Test", tr("TXT (*.txt)"));
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out (&file);
    QString text;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        text = text + ui->listWidget->item(i)->text() + "\n";
    }
    //Кодируем текст
    QByteArray arrayResult;
    arrayResult.insert(0,text.toUtf8());
    QTextCodec *codec1251 = QTextCodec::codecForName("Windows-1251");
    QString textUnicode = codec1251->toUnicode(arrayResult);
    QTextCodec *codecUtf8=QTextCodec::codecForName("UTF-8");
    QString textUtf = codecUtf8->fromUnicode(textUnicode);
    //Конец кодирования
    out << textUtf;
    file.close();
    modelClear();
}

//Отчистить модель
void MainWindow::modelClear(){
    ControllerApp *newAppController = new ControllerApp();
    newAppController->clearModel();
    updateListWidget();
    ui->listWidget->takeItem(0);
    setWindowTitle("No file");
    delete newAppController;
}

//Новый файл
void MainWindow::on_action_2_triggered()
{
    modelClear();
    setWindowTitle("New File");
}

//Удаление вопроса
void MainWindow::on_pushButton_8_clicked()
{
    ControllerApp *newAppController = new ControllerApp();
    int index = ui->listWidget->currentRow();
    if (index != -1 ) {
        newAppController->removeQuest(index);
        updateListWidget();
    }
    delete newAppController;
}

//Печать
void MainWindow::on_action_3_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer");
    QPrintDialog pDialog(&printer, this);
    if (pDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this,"Ошибка","Не удалось обратиться к принтеру");
        return;
    }
    QString text;
    for (int i = 0; i < ui->listWidget->count(); i ++) {
        text = text + ui->listWidget->item(i)->text();
    }
    QPainter painter;
    painter.begin(&printer);
    painter.drawText(100, 100, 500, 500, Qt::AlignLeft|Qt::AlignTop, text);
    painter.end();
}

