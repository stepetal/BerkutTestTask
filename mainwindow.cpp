#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      conveyerStateMachine(std::make_unique<ConveyerStateMachine>())
{
    createWidgets();
    createLayout();
    createConnections();

}

MainWindow::~MainWindow()
{
}

void MainWindow::createWidgets()
{
    startWorkPushButton = new QPushButton("Запуск");
    resetPushButton = new QPushButton("Сброс");
    resumeWorkPushButton = new QPushButton("Исправить");
    endWorkPushButton = new QPushButton("End");
    terminatePushButton = new QPushButton("Завершение");
    addActuatingMechanismPushButton = new QPushButton("Добавить исполнительный механизм");
    numberOfActuatingMechanismsLineEdit = new QLineEdit();
    numberOfActuatingMechanismsLineEdit->setReadOnly(true);
    numberOfActuatingMechanismsLineEdit->setText(QString("0"));
    maximumNumberOfActuatingMechanismsLineEdit = new QLineEdit();
    maximumNumberOfActuatingMechanismsLineEdit->setReadOnly(true);
    maximumNumberOfActuatingMechanismsLineEdit->setText(QString::number(conveyerStateMachine->getMaximumNumberOfActuatingMechanisms()));
    tasksComboBox = new QComboBox();
    tasksComboBox->addItem(QString("Задача без ошибок"));
    tasksComboBox->addItem(QString("Задача с ошибками"));

    //изначально активна только кнопка добавления исполнительного механизма
    addActuatingMechanismPushButton->setStyleSheet("background-color: #0e7fc9; color: white; font-size: 12pt"); // enabled style sheet
    terminatePushButton->setEnabled(false);
    terminatePushButton->setStyleSheet("background-color: #437ca1; color: white; font-size: 12pt"); // disabled style sheet
    resetPushButton->setEnabled(false);
    resetPushButton->setStyleSheet("background-color: #437ca1; color: white; font-size: 12pt"); // disabled style sheet
    startWorkPushButton->setEnabled(false);
    startWorkPushButton->setStyleSheet("background-color: #437ca1; color: white; font-size: 12pt"); // disabled style sheet
    resumeWorkPushButton->setEnabled(false);
    resumeWorkPushButton->setStyleSheet("background-color: #437ca1; color: white; font-size: 12pt"); // disabled style sheet
}

void MainWindow::createLayout()
{
    QWidget *mainWindowWidget = new QWidget();
    QGridLayout *mainWindowWidgetLayout = new QGridLayout();

    QLabel *title = new QLabel("Приложение \"Имитатор сборочного конвейера\"");

    QGroupBox *actuatingMechanismsSettingGroupBox = new QGroupBox("Управление исполняемыми механизмами");
    QGridLayout *actuatingMechanismsSettingGroupBoxLayout = new QGridLayout();
    QLabel *currentNumberOfActuatingMechanismsLabel = new QLabel("Текущее количество исполнительных механизмов");
    QLabel *maximumNumberOfActuatingMechanismsLabel = new QLabel("Максимальное количество исполнительных механизмов");
    QLabel *currentTaskLabel = new QLabel("Задача исполнительного механизма");
    actuatingMechanismsSettingGroupBoxLayout->addWidget(currentNumberOfActuatingMechanismsLabel,0,0,1,1);
    actuatingMechanismsSettingGroupBoxLayout->addWidget(numberOfActuatingMechanismsLineEdit,0,1,1,1);
    actuatingMechanismsSettingGroupBoxLayout->addWidget(maximumNumberOfActuatingMechanismsLabel,1,0,1,1);
    actuatingMechanismsSettingGroupBoxLayout->addWidget(maximumNumberOfActuatingMechanismsLineEdit,1,1,1,1);
    actuatingMechanismsSettingGroupBoxLayout->addWidget(currentTaskLabel,2,0,1,1);
    actuatingMechanismsSettingGroupBoxLayout->addWidget(tasksComboBox,2,1,1,1);
    actuatingMechanismsSettingGroupBoxLayout->addWidget(addActuatingMechanismPushButton,3,0,1,2,Qt::AlignCenter);
    actuatingMechanismsSettingGroupBox->setLayout(actuatingMechanismsSettingGroupBoxLayout);

    QGroupBox *operatorControlGroupBox = new QGroupBox("Панель оператора");
    QHBoxLayout *operatorControlGroupBoxLayout = new QHBoxLayout();
    operatorControlGroupBoxLayout->addWidget(startWorkPushButton);
    operatorControlGroupBoxLayout->addWidget(resumeWorkPushButton);
    operatorControlGroupBoxLayout->addWidget(resetPushButton);
    operatorControlGroupBoxLayout->addWidget(terminatePushButton);
    operatorControlGroupBox->setLayout(operatorControlGroupBoxLayout);

    mainWindowWidgetLayout->addWidget(title,0,0,1,2,Qt::AlignCenter);
    mainWindowWidgetLayout->addWidget(actuatingMechanismsSettingGroupBox,1,0,1,2);
    mainWindowWidgetLayout->addWidget(operatorControlGroupBox,2,0,1,2);

    //установка стилей
    title->setFont(QFont("Arial",16,QFont::Bold,true));
    actuatingMechanismsSettingGroupBox->setFont(QFont(QString("Arial"),12,-1,true));
    operatorControlGroupBox->setFont(QFont(QString("Arial"),12,-1,true));
    mainWindowWidget->setStyleSheet("background-color: #0e7fc9; color: white;");
    actuatingMechanismsSettingGroupBox->setStyleSheet("background-color: #7ec4f2");
    operatorControlGroupBox->setStyleSheet("background-color: #7ec4f2");
    numberOfActuatingMechanismsLineEdit->setStyleSheet("background-color: white; color: black");
    maximumNumberOfActuatingMechanismsLineEdit->setStyleSheet("background-color: white; color: black");
    tasksComboBox->setStyleSheet("background-color: white; color: black");

    mainWindowWidget->setLayout(mainWindowWidgetLayout);
    setCentralWidget(mainWindowWidget);
}

void MainWindow::createConnections()
{
    connect(addActuatingMechanismPushButton,&QPushButton::clicked,[&]()
    {
        if(numberOfActuatingMechanismsLineEdit->text().toInt() < maximumNumberOfActuatingMechanismsLineEdit->text().toInt())
        {
            if(tasksComboBox->currentIndex() == 0)
            {
                conveyerStateMachine->addActuatingMechanism(SYS::enum_actuating_mechanism_state::NORMAL);
            }
            if(tasksComboBox->currentIndex() == 1)
            {
                conveyerStateMachine->addActuatingMechanism(SYS::enum_actuating_mechanism_state::FAULTY);
            }
            numberOfActuatingMechanismsLineEdit->setText(QString::number(numberOfActuatingMechanismsLineEdit->text().toInt() + 1));

            //если имеется один исполнительный механизм, делаем кнопки управления активными
            if(numberOfActuatingMechanismsLineEdit->text().toInt() == 1)
            {
                terminatePushButton->setEnabled(true);
                resetPushButton->setEnabled(true);
                startWorkPushButton->setEnabled(true);
                resumeWorkPushButton->setEnabled(true);
                startWorkPushButton->setStyleSheet("background-color: #0e7fc9; color: white; font-size: 12pt"); // enabled style sheet
                resumeWorkPushButton->setStyleSheet("background-color: #0e7fc9; color: white; font-size: 12pt"); // enabled style sheet
                resetPushButton->setStyleSheet("background-color: #0e7fc9; color: white; font-size: 12pt"); // enabled style sheet
                terminatePushButton->setStyleSheet("background-color: #0e7fc9; color: white; font-size: 12pt"); // enabled style sheet
            }
        }
    });

    connect(startWorkPushButton,&QPushButton::clicked,[&]()
    {
        addActuatingMechanismPushButton->setEnabled(false);
        addActuatingMechanismPushButton->setStyleSheet("background-color: #437ca1; color: white; font-size: 12pt"); // disabled style sheet
        conveyerStateMachine->startWork();
    });

    connect(resetPushButton,&QPushButton::clicked,[&]()
    {
        addActuatingMechanismPushButton->setEnabled(true);
        addActuatingMechanismPushButton->setStyleSheet("background-color: #0e7fc9; color: white; font-size: 12pt"); // enabled style sheet
        terminatePushButton->setEnabled(false);
        terminatePushButton->setStyleSheet("background-color: #437ca1; color: white; font-size: 12pt"); // disabled style sheet
        resetPushButton->setEnabled(false);
        resetPushButton->setStyleSheet("background-color: #437ca1; color: white; font-size: 12pt"); // disabled style sheet
        startWorkPushButton->setEnabled(false);
        startWorkPushButton->setStyleSheet("background-color: #437ca1; color: white; font-size: 12pt"); // disabled style sheet
        resumeWorkPushButton->setEnabled(false);
        resumeWorkPushButton->setStyleSheet("background-color: #437ca1; color: white; font-size: 12pt"); // disabled style sheet
        numberOfActuatingMechanismsLineEdit->setText(QString("0"));
        conveyerStateMachine->resetConveyer();
    });

    connect(resumeWorkPushButton,&QPushButton::clicked,[&]()
    {
        conveyerStateMachine->resumeWorkAfterFault();
    });

    connect(terminatePushButton,&QPushButton::clicked,[&]()
    {
        conveyerStateMachine->terminateWork();
    });

    connect(conveyerStateMachine.get(),&ConveyerStateMachine::updateConveyerState,[&](std::tuple<int,SYS::enum_status> info)
    {
        statusBar()->showMessage(QString(SYS::enumToString(std::get<1>(info)) + QString("; Текущий цикл работы: ") + QString::number(std::get<0>(info))));
    });
}

