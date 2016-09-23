#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T12:40:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PatientMonitorDemo
TEMPLATE = app

DESTDIR     = Release
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR     = $$DESTDIR/.moc
RCC_DIR     = $$DESTDIR/.qrc
UI_DIR      = $$DESTDIR/.ui


##########################################
#
# Demo Configurations:
#        PMD_HAMP = Heterogeneous AMP - Patient Monitor Demo with MEHV and MEMF on OMAP5
#        PMD_MEHV = Patient Monitor Demo with MEHV on OMAP5
#        PMD_NUCLEUS= Patient Monitor Demo with Nucleus(GPU) on imx6 Saber lite with LVDS
#
#   - Select appropriate OS
#       PMD_NUCLEUS
#       PMD_MEHV
#       PMD_HAMP
DEFINES += PMD_DDS

#   - The demo can be used with one of the design size
#       DESIGN_SIZE_XGA
#       DESIGN_SIZE_720p
DEFINES += DESIGN_SIZE_720p

#   - Un-comment following to enable graph scrolling on demo startup
#DEFINES += ENABLE_GRAPH_SCROLLING
#
################
#
# For following options are purely for development purposes:
#
#   - Only show ECG graph
#DEFINES += ONLY_SHOW_ECG_GRAPH

#   - Disable numeric values update in the right panel
#DEFINES += DISABLE_RIGHT_PANEL_NUMERIC_VALUES

#
##########################################

contains(DEFINES,PMD_MEHV|PMD_HAMP|PMD_REMOTE) {
    QT += network
}


SOURCES +=  main.cpp\
            mainwindow.cpp \
            widget.cpp \
            helper.cpp \
            graphSettingsPopup.cpp

HEADERS  += mainwindow.h \
            widget.h \
            helper.h \
            graphSettingsPopup.h \
            data.h \

FORMS   +=  Common/graphSettingsPopup.ui
RESOURCES += Common/graphics.qrc

contains(DEFINES, PMD_MEHV) {
    SOURCES     +=  MEHV_dataSupplier.cpp
    HEADERS     +=  MEHV_dataSupplier.h

    contains(DEFINES,PMD_NUCLEUS) {
        message(====== Error: Please define only one configuration (PMD_MEHV OR PMD_NUCLEUS)  =====)
    }
}

contains(DEFINES, PMD_NUCLEUS) {
    SOURCES     += NUC_dataSupplier.cpp
    HEADERS     += NUC_dataSupplier.h
}

contains(DEFINES, PMD_HAMP) {
    SOURCES     +=  HAMP_dataSupplier.cpp
    HEADERS     +=  HAMP_dataSupplier.h
}

contains(DEFINES, PMD_REMOTE) {
    SOURCES     +=  remote_datasupplier.cpp
	HEADERS     +=  remote_datasupplier.h
}

contains(DEFINES, PMD_DDS) {
    SOURCES     += dds_datasupplier.cpp pmd.cpp  pmdPlugin.cpp \
  				   pmd_subscriber.cpp  pmdSupport.cpp
    HEADERS     += dds_datasupplier.h pmd.h pmdPlugin.h pmdSupport.h
}

contains(DEFINES, DESIGN_SIZE_XGA) {
  #  FORMS       += XGA/mainwindow.ui
  #  RESOURCES   += XGA/images.qrc
}

contains(DEFINES, DESIGN_SIZE_720p) {
    FORMS       += 720p/mainwindow.ui
    RESOURCES   += 720p/images.qrc

    contains(DEFINES,DESIGN_SIZE_XGA){
        message(====== Error: Please define only one Design Size (XGA or 720p)  =====)
    }
}

contains(DEFINES, PMD_HAMP|PMD_REMOTE) {

target.path=$$[QT_INSTALL_EXAMPLES]/HAMPDemo/PatientMonitorDemo
INSTALLS +=target
}


