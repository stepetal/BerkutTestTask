#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QStatusBar>
#include <memory>
#include <tuple>
#include "conveyerstatemachine.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPushButton *startWorkPushButton;
    QPushButton *resetPushButton;
    QPushButton *resumeWorkPushButton;
    QPushButton *endWorkPushButton;
    QPushButton *terminatePushButton;
    QLineEdit *numberOfActuatingMechanismsLineEdit;
    QLineEdit *maximumNumberOfActuatingMechanismsLineEdit;
    QComboBox *tasksComboBox;
    QPushButton *addActuatingMechanismPushButton;
    std::unique_ptr<ConveyerStateMachine> conveyerStateMachine;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createWidgets();
    void createLayout();
    void createConnections();
};
#endif // MAINWINDOW_H
