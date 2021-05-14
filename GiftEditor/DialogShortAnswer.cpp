#include "DialogShortAnswer.h"
#include "ui_shortanswerdialog.h"
#include "ControllerApp.h"

ShortAnswerDialog::ShortAnswerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShortAnswer)
{
    ui->setupUi(this);
}

ShortAnswerDialog::~ShortAnswerDialog()
{
    delete ui;
}

void ShortAnswerDialog::setUpUI(QStringList data) {
    ui->lineEdit->setText(data[0]);
    ui->lineEdit_2->setText(data[1]);
    ui->lineEdit_3->setText(data[2]);
}
void ShortAnswerDialog::on_pushButton_clicked()
{
    ControllerApp *newAppController = new ControllerApp();
    if (editable) {
            QStringList resultList;
            resultList.push_back(ui->lineEdit->text());
            resultList.push_back(ui->lineEdit_2->text());
            resultList.push_back("=" + ui->lineEdit_3->text());
            newAppController->editQuest(index,"Short Answer", resultList);
        } else {
            newAppController->sendModelShortAnswer(ui->lineEdit->text(), ui->lineEdit_2->text(),"=" + ui->lineEdit_3->text());
        }
    delete newAppController;
    this->close();
}

