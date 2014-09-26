#include "engMenuPopup.h"
#include "mainwindow.h"
#include "ui_engMenuPopup.h"

EngMenuPopup::EngMenuPopup(MainWindow *parent):
    ui(new Ui::EngMenuPopup),
    m_mainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);
    //this->setAttribute(Qt::WA_TranslucentBackground );
    ui->setupUi(this);
    /*
#ifdef DESIGN_SIZE_720p
    this->resize(1280,720);
    ui->pbBackground->resize(1280,720);
#endif
#ifdef DESIGN_SIZE_XGA
    this->resize(1024,768);
    ui->pbBackground->resize(1024,768);
#endif
    ui->frame->move( width()/2 - ui->frame->width()/2 , height()/2 - ui->frame->height()/2);
*/
    // this->move(360,200);
    m_isVisible=false;
    m_scrollingStatus=false;
    m_nucluesStatus=false;
    connect(ui->okButton, SIGNAL(clicked()), m_mainWindow, SLOT(onEngMenuPopupOk()));
    connect(ui->cancelButton, SIGNAL(clicked()), m_mainWindow, SLOT(onEngMenuPopupCancel()));
}

EngMenuPopup::~EngMenuPopup()
{
    delete ui;
}

void EngMenuPopup::initialized(bool scrollOption )
{
    m_scrollingStatus=scrollOption;
    m_previousScrollingStatus=m_scrollingStatus;
    m_previousNucleusState=m_nucluesStatus;

    m_isVisible=true;

#ifndef PMD_HAMP
    ui->btn_Nucleus->setEnabled(false);
    ui->lbnucleus->setEnabled(false);
#endif

}

void EngMenuPopup::on_btn_Nucleus_clicked(bool checked)
{
    (void)checked;
    m_nucluesStatus=!m_nucluesStatus;
}

void EngMenuPopup::on_btn_Scrolling_clicked(bool checked)
{
    if(checked)
    {
        m_scrollingStatus=!m_scrollingStatus;
    }
    else
    {
        m_scrollingStatus=!m_scrollingStatus;
    }
}

bool EngMenuPopup::isScrollingStateChanged(bool &scrollingState)
{
    scrollingState=m_scrollingStatus;
    return m_previousScrollingStatus!=m_scrollingStatus;
}

bool EngMenuPopup::isNucleusStateChanged(bool &nucleusState)
{
    nucleusState=m_nucluesStatus;
    return m_previousNucleusState!=m_nucluesStatus;
}
