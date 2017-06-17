#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "recording/coordinator.h"

#include <QStandardPaths>
#include <QDateTime>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_recorder = new Recording::Coordinator(this);
    QObject::connect(m_recorder, &Recording::Coordinator::statusUpdate, ui->statusView, &Recording::StatusView::handleStatusUpdate);
    QObject::connect(m_recorder, &Recording::Coordinator::error, ui->errorWidget, &Recording::ErrorWidget::displayError);

    QObject::connect(ui->bEnableMonitor, &QAbstractButton::toggled, m_recorder, &Recording::Coordinator::setMonitorEnabled);
    QObject::connect(ui->bEnableRecord, &QAbstractButton::toggled, m_recorder, &Recording::Coordinator::setRecording);
    QObject::connect(ui->bNewTrack, &QAbstractButton::clicked, m_recorder, &Recording::Coordinator::startNewTrack);
    QObject::connect(m_recorder, &Recording::Coordinator::recordingChanged, ui->bEnableRecord, &QAbstractButton::setChecked);
    QObject::connect(m_recorder, &Recording::Coordinator::recordingChanged, ui->bNewTrack, &QAbstractButton::setEnabled);
    QObject::connect(m_recorder, &Recording::Coordinator::monitorEnabledChanged, ui->bEnableMonitor, &QAbstractButton::setChecked);

    QObject::connect(ui->bDebugCrash, &QAbstractButton::clicked, this, &MainWindow::crashMe);

    ui->configPane->hookupCoordinator(m_recorder);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::crashMe() const
{
    // UB here we come...
    *((unsigned int*)0) = 0xDEAD;
}
