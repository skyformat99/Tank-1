#include "qeditform.h"
#include "ui_qeditform.h"
#include <QDebug>
#include "qtankpublic.h"
#include "qtankdefine.h"
#include <QSqlQuery>
#include <QItemSelection>

QEditForm::QEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QEditForm)
{
    ui->setupUi(this);

    openLib = new QUserLibraryDialog(this);
    tbvStage = new QStageWidget(this);
    tbvStage->setHidden(true);
    tbvStage->addMap(ui->le_timeramp, Stage_Timeramp);
    tbvStage->addMap(ui->le_tempture, Stage_Tempture);
    tbvStage->addMap(ui->le_press, Stage_Presspsi);
    tbvStage->addMap(ui->le_hold, Stage_Hold);

    QRegExp regExp("[0-9]{1,2}[:][0-9]{0,2}");
    QRegExpValidator *pValidator = new QRegExpValidator(regExp, this);
    ui->le_timeramp->setValidator(pValidator);
    ui->le_timeramp->setInputMask("00:00");
    ui->le_hold->setValidator(pValidator);
    ui->le_hold->setInputMask("00:00");
    libForm = new QLibraryForm(this);

    connect(ui->tableView_method->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentMethodChanged(QModelIndex,QModelIndex)));

    connect(ui->pushButton_method_add, SIGNAL(clicked()), this, SLOT(newM()));
    connect(ui->pushButton_method_del, SIGNAL(clicked()), this, SLOT(delM()));
    connect(ui->pushButton_method_save, SIGNAL(clicked()), this, SLOT(saveM()));
    connect(libForm, SIGNAL(libSelectedDisplayed(QString)), ui->label_method_libname, SLOT(setText(QString)));
    connect(libForm, SIGNAL(libSelected(QString)), this, SLOT(libGetted(QString)));
}

QEditForm::~QEditForm()
{
    delete ui;
}

void QEditForm::initAll()
{
}

void QEditForm::libGetted(QString name)
{
    if(DB_EXTRACT == name)
    {
        ui->lb_pressBuddy->setHidden(true);
        ui->lb_timerampBuddy->setHidden(true);
        ui->lbTypeBuddy->setHidden(true);
        ui->le_press->setHidden(true);
        ui->le_timeramp->setHidden(true);
        ui->comboBox_method_type->setHidden(true);
    }
    else
    {
        ui->lb_pressBuddy->setHidden(false);
        ui->lb_timerampBuddy->setHidden(false);
        ui->lbTypeBuddy->setHidden(false);
        ui->le_press->setHidden(false);
        ui->le_timeramp->setHidden(false);
        ui->comboBox_method_type->setHidden(false);
    }

    ui->tableView_method->initdb(name);
    tbvStage->initdb(name);

    ui->tableView_method->refresh();
    ui->tableView_method->selectMethod();
}

void QEditForm::currentMethodChanged(QModelIndex, QModelIndex)
{
    ui->lineEdit_methodname->setText(ui->tableView_method->currentMethodName());
    ui->comboBox_method_type->setCurrentIndex(ui->tableView_method->currentMethodType());
    ui->le_vessel->setText(ui->tableView_method->currentMethodVessel());

    tbvStage->setMethodId(ui->tableView_method->currentMethodId());
    tbvStage->refresh(ui->tableView_method->currentMethodId());

    if(Type_Extract == ui->tableView_method->currentMethodType())
    {
        ui->lb_timerampBuddy->setHidden(true);
        ui->le_timeramp->setHidden(true);
        ui->lb_pressBuddy->setHidden(true);
        ui->le_press->setHidden(true);
        ui->lb_temptureBuddy->setHidden(false);
        ui->le_tempture->setHidden(false);
    }
    else if(Type_Standard == ui->tableView_method->currentMethodType())
    {
        ui->lb_timerampBuddy->setHidden(true);
        ui->le_timeramp->setHidden(true);
        ui->lb_pressBuddy->setHidden(true);
        ui->le_press->setHidden(true);
        ui->lb_temptureBuddy->setHidden(false);
        ui->le_tempture->setHidden(false);
    }
    else if(Type_Stressure == ui->tableView_method->currentMethodType())
    {
        ui->lb_timerampBuddy->setHidden(true);
        ui->le_timeramp->setHidden(true);
        ui->lb_pressBuddy->setHidden(false);
        ui->le_press->setHidden(false);
        ui->lb_temptureBuddy->setHidden(true);
        ui->le_tempture->setHidden(true);
    }
    else if(Type_Temprature == ui->tableView_method->currentMethodType())
    {
        ui->lb_timerampBuddy->setHidden(false);
        ui->le_timeramp->setHidden(false);
        ui->lb_pressBuddy->setHidden(false);
        ui->le_press->setHidden(false);
        ui->lb_temptureBuddy->setHidden(false);
        ui->le_tempture->setHidden(false);
    }


    ui->spin_stage->setRange(1, tbvStage->countStage());
    ui->spin_stage->setValue(1);
    ui->le_stagenum->setText(QString::number(tbvStage->countStage()));
}

void QEditForm::saveM()
{
    int stage = ui->spin_stage->value();
    int type = ui->comboBox_method_type->currentIndex();
    int vessel = ui->le_vessel->text().toInt();
    QString newName = ui->lineEdit_methodname->text();
    QModelIndex index = ui->tableView_method->currentIndex();
    ui->tableView_method->setMethodType(index.row(), type);
    ui->tableView_method->setMethodName(index.row(), newName);
    ui->tableView_method->setMethodVessel(index.row(), vessel);
    pline() << index.row() << type << newName;

    QStringList rampList = ui->le_timeramp->text().split(":");
    quint16 ramp = rampList[0].toUInt() * 60 + rampList[1].toUInt();
    quint16 press = ui->le_press->text().toUInt();
    quint16 tempture = ui->le_tempture->text().toUInt();
    QStringList holdList = ui->le_hold->text().split(":");
    quint16 hold = holdList[0].toUInt() * 60 + holdList[1].toUInt();
    pline() << stage << ramp << press << tempture << hold;
    tbvStage->saveStage(stage, vessel, ramp, press, tempture, hold);
    ui->spin_stage->setValue(stage);
}

void QEditForm::delM()
{
    tbvStage->delAllStage();
    ui->tableView_method->delMethod(ui->tableView_method->currentIndex().row());
}

void QEditForm::newM()
{
    ui->tableView_method->newMethod();
    tbvStage->setMethodId(ui->tableView_method->currentMethodId());
    tbvStage->newStage();
}

void QEditForm::initLanguage()
{
    ui->retranslateUi(this);
    ui->comboBox_method_type->clear();
    QStringList methodType;
    methodType << tr("Standard");
    methodType << tr("Tempture");
    methodType << tr("Pressure");
    ui->comboBox_method_type->addItems(methodType);
    ui->comboBox_method_type->setCurrentIndex(0);
}

void QEditForm::on_btn_open_clicked()
{
    libForm->initAll();
    libForm->exec();
}

void QEditForm::on_spin_stage_valueChanged(int arg1)
{
    pline() << tbvStage->currentStage() << arg1;
    tbvStage->selectStage(arg1-1);
    pline() << tbvStage->currentStage();
}

void QEditForm::on_btn_stage_clear_clicked()
{
    tbvStage->cleanStage();
    ui->spin_stage->setRange(1, 1);
    ui->spin_stage->setValue(1);
}

void QEditForm::on_btn_stage_del_clicked()
{
    tbvStage->delStage();
    ui->spin_stage->setRange(1, tbvStage->countStage());
    ui->spin_stage->setValue(tbvStage->countStage());
    ui->le_stagenum->setText(QString::number(tbvStage->countStage()));
}

void QEditForm::on_btn_stage_add_clicked()
{
    tbvStage->newStage();
    ui->spin_stage->setRange(1, tbvStage->countStage());
    ui->spin_stage->setValue(tbvStage->countStage());
    ui->le_stagenum->setText(QString::number(tbvStage->countStage()));
}

