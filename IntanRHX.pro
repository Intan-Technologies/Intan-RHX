#-------------------------------------------------
#
# Project created by QtCreator 2019-01-16T15:10:30
#
#-------------------------------------------------

QT       += core gui xml multimedia network widgets

TARGET = IntanRHX
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/Engine/Processing/
INCLUDEPATH += $$PWD/Engine/Processing/DataFileReaders/
INCLUDEPATH += $$PWD/Engine/Processing/SaveManagers/
INCLUDEPATH += $$PWD/Engine/Processing/XPUInterfaces/
INCLUDEPATH += $$PWD/Engine/Threads/
INCLUDEPATH += $$PWD/Engine/API/Synthetic/
INCLUDEPATH += $$PWD/Engine/API/Abstract/
INCLUDEPATH += $$PWD/Engine/API/Hardware/
INCLUDEPATH += $$PWD/GUI/Dialogs/
INCLUDEPATH += $$PWD/GUI/Widgets/
INCLUDEPATH += $$PWD/GUI/Windows/


SOURCES += main.cpp \
    Engine/API/Synthetic/playbackrhxcontroller.cpp \
    Engine/API/Synthetic/randomnumber.cpp \
    Engine/API/Synthetic/synthdatablockgenerator.cpp \
    Engine/API/Synthetic/syntheticrhxcontroller.cpp \
    Engine/API/Abstract/abstractrhxcontroller.cpp \
    Engine/API/Hardware/rhxcontroller.cpp \
    Engine/API/Hardware/rhxdatablock.cpp \
    Engine/API/Hardware/rhxregisters.cpp \
    Engine/Processing/DataFileReaders/datafile.cpp \
    Engine/Processing/DataFileReaders/datafilemanager.cpp \
    Engine/Processing/DataFileReaders/datafilereader.cpp \
    Engine/Processing/DataFileReaders/fileperchannelmanager.cpp \
    Engine/Processing/DataFileReaders/filepersignaltypemanager.cpp \
    Engine/Processing/DataFileReaders/traditionalintanfilemanager.cpp \
    Engine/Processing/SaveManagers/fileperchannelsavemanager.cpp \
    Engine/Processing/SaveManagers/filepersignaltypesavemanager.cpp \
    Engine/Processing/SaveManagers/intanfilesavemanager.cpp \
    Engine/Processing/SaveManagers/savefile.cpp \
    Engine/Processing/SaveManagers/savemanager.cpp \
    Engine/Processing/XPUInterfaces/abstractxpuinterface.cpp \
    Engine/Processing/XPUInterfaces/cpuinterface.cpp \
    Engine/Processing/XPUInterfaces/gpuinterface.cpp \
    Engine/Processing/XPUInterfaces/xpucontroller.cpp \
    Engine/Processing/channel.cpp \
    Engine/Processing/commandparser.cpp \
    Engine/Processing/controllerinterface.cpp \
    Engine/Processing/datastreamfifo.cpp \
    Engine/Processing/displayundomanager.cpp \
    Engine/Processing/fastfouriertransform.cpp \
    Engine/Processing/filter.cpp \
    Engine/Processing/matfilewriter.cpp \
    Engine/Processing/rhxdatareader.cpp \
    Engine/Processing/signalsources.cpp \
    Engine/Processing/softwarereferenceprocessor.cpp \
    Engine/Processing/stateitem.cpp \
    Engine/Processing/stimparameters.cpp \
    Engine/Processing/stimparametersclipboard.cpp \
    Engine/Processing/systemstate.cpp \
    Engine/Processing/tcpcommunicator.cpp \
    Engine/Processing/waveformfifo.cpp \
    Engine/Processing/impedancereader.cpp \
    Engine/Processing/xmlinterface.cpp \
    Engine/Threads/audiothread.cpp \
    Engine/Threads/savetodiskthread.cpp \
    Engine/Threads/tcpdataoutputthread.cpp \
    Engine/Threads/usbdatathread.cpp \
    Engine/Threads/waveformprocessorthread.cpp \
    GUI/Dialogs/ampsettledialog.cpp \
    GUI/Dialogs/analogoutconfigdialog.cpp \
    GUI/Dialogs/anoutdialog.cpp \
    GUI/Dialogs/autocolordialog.cpp \
    GUI/Dialogs/autogroupdialog.cpp \
    GUI/Dialogs/auxdigoutconfigdialog.cpp \
    GUI/Dialogs/bandwidthdialog.cpp \
    GUI/Dialogs/boardselectdialog.cpp \
    GUI/Dialogs/cabledelaydialog.cpp \
    GUI/Dialogs/chargerecoverydialog.cpp \
    GUI/Dialogs/demodialog.cpp \
    GUI/Dialogs/digoutdialog.cpp \
    GUI/Dialogs/isidialog.cpp \
    GUI/Dialogs/keyboardshortcutdialog.cpp \
    GUI/Dialogs/performanceoptimizationdialog.cpp \
    GUI/Dialogs/playbackfilepositiondialog.cpp \
    GUI/Dialogs/psthdialog.cpp \
    GUI/Dialogs/referenceselectdialog.cpp \
    GUI/Dialogs/renamechanneldialog.cpp \
    GUI/Dialogs/setfileformatdialog.cpp \
    GUI/Dialogs/setthresholdsdialog.cpp \
    GUI/Dialogs/spectrogramdialog.cpp \
    GUI/Dialogs/spikesortingdialog.cpp \
    GUI/Dialogs/startupdialog.cpp \
    GUI/Dialogs/stimparamdialog.cpp \
    GUI/Dialogs/triggerrecorddialog.cpp \
    GUI/Dialogs/waveformselectdialog.cpp \
    GUI/Widgets/abstractfigure.cpp \
    GUI/Widgets/anoutfigure.cpp \
    GUI/Widgets/controlpanelaudioanalogtab.cpp \
    GUI/Widgets/controlpanelbandwidthtab.cpp \
    GUI/Widgets/controlpanelconfiguretab.cpp \
    GUI/Widgets/controlpanelimpedancetab.cpp \
    GUI/Widgets/controlpaneltriggertab.cpp \
    GUI/Widgets/digfigure.cpp \
    GUI/Widgets/displayedwaveform.cpp \
    GUI/Widgets/displaylistmanager.cpp \
    GUI/Widgets/filterdisplayselector.cpp \
    GUI/Widgets/filterplot.cpp \
    GUI/Widgets/impedancegradient.cpp \
    GUI/Widgets/isiplot.cpp \
    GUI/Widgets/multicolumndisplay.cpp \
    GUI/Widgets/multiwaveformplot.cpp \
    GUI/Widgets/pageview.cpp \
    GUI/Widgets/plotutilities.cpp \
    GUI/Widgets/psthplot.cpp \
    GUI/Widgets/scrollbar.cpp \
    GUI/Widgets/smartspinbox.cpp \
    GUI/Widgets/spikegradient.cpp \
    GUI/Widgets/spikeplot.cpp \
    GUI/Widgets/statusbars.cpp \
    GUI/Widgets/stimfigure.cpp \
    GUI/Widgets/tcpdisplay.cpp \
    GUI/Widgets/voltagespinbox.cpp \
    GUI/Widgets/waveformdisplaycolumn.cpp \
    GUI/Widgets/waveformdisplaymanager.cpp \
    GUI/Windows/controlwindow.cpp \
    GUI/Windows/probemapwindow.cpp \
    GUI/Dialogs/impedancefreqdialog.cpp \
    GUI/Widgets/controlpanel.cpp \
    GUI/Widgets/spectrogramplot.cpp \
    GUI/Windows/viewfilterswindow.cpp


HEADERS += \
    Engine/API/Synthetic/playbackrhxcontroller.h \
    Engine/API/Synthetic/randomnumber.h \
    Engine/API/Synthetic/synthdatablockgenerator.h \
    Engine/API/Synthetic/syntheticrhxcontroller.h \
    Engine/API/Abstract/abstractrhxcontroller.h \
    Engine/API/Hardware/rhxcontroller.h \
    Engine/API/Hardware/rhxdatablock.h \
    Engine/API/Hardware/rhxglobals.h \
    Engine/API/Hardware/rhxregisters.h \
    Engine/Processing/DataFileReaders/datafile.h \
    Engine/Processing/DataFileReaders/datafilemanager.h \
    Engine/Processing/DataFileReaders/datafilereader.h \
    Engine/Processing/DataFileReaders/fileperchannelmanager.h \
    Engine/Processing/DataFileReaders/filepersignaltypemanager.h \
    Engine/Processing/DataFileReaders/traditionalintanfilemanager.h \
    Engine/Processing/SaveManagers/fileperchannelsavemanager.h \
    Engine/Processing/SaveManagers/filepersignaltypesavemanager.h \
    Engine/Processing/SaveManagers/intanfilesavemanager.h \
    Engine/Processing/SaveManagers/savefile.h \
    Engine/Processing/SaveManagers/savemanager.h \
    Engine/Processing/XPUInterfaces/abstractxpuinterface.h \
    Engine/Processing/XPUInterfaces/cpuinterface.h \
    Engine/Processing/XPUInterfaces/gpuinterface.h \
    Engine/Processing/XPUInterfaces/xpucontroller.h \
    Engine/Processing/channel.h \
    Engine/Processing/commandparser.h \
    Engine/Processing/controllerinterface.h \
    Engine/Processing/datastreamfifo.h \
    Engine/Processing/displayundomanager.h \
    Engine/Processing/fastfouriertransform.h \
    Engine/Processing/filter.h \
    Engine/Processing/matfilewriter.h \
    Engine/Processing/minmax.h \
    Engine/Processing/probemapdatastructures.h \
    Engine/Processing/rhxdatareader.h \
    Engine/Processing/semaphore.h \
    Engine/Processing/signalsources.h \
    Engine/Processing/softwarereferenceprocessor.h \
    Engine/Processing/stateitem.h \
    Engine/Processing/stimparameters.h \
    Engine/Processing/stimparametersclipboard.h \
    Engine/Processing/systemstate.h \
    Engine/Processing/tcpcommunicator.h \
    Engine/Processing/waveformfifo.h \
    Engine/Processing/impedancereader.h \
    Engine/Processing/xmlinterface.h \
    Engine/Threads/audiothread.h \
    Engine/Threads/savetodiskthread.h \
    Engine/Threads/tcpdataoutputthread.h \
    Engine/Threads/usbdatathread.h \
    Engine/Threads/waveformprocessorthread.h \
    GUI/Dialogs/ampsettledialog.h \
    GUI/Dialogs/analogoutconfigdialog.h \
    GUI/Dialogs/anoutdialog.h \
    GUI/Dialogs/autocolordialog.h \
    GUI/Dialogs/autogroupdialog.h \
    GUI/Dialogs/auxdigoutconfigdialog.h \
    GUI/Dialogs/bandwidthdialog.h \
    GUI/Dialogs/boardselectdialog.h \
    GUI/Dialogs/cabledelaydialog.h \
    GUI/Dialogs/chargerecoverydialog.h \
    GUI/Dialogs/demodialog.h \
    GUI/Dialogs/digoutdialog.h \
    GUI/Dialogs/isidialog.h \
    GUI/Dialogs/keyboardshortcutdialog.h \
    GUI/Dialogs/performanceoptimizationdialog.h \
    GUI/Dialogs/playbackfilepositiondialog.h \
    GUI/Dialogs/psthdialog.h \
    GUI/Dialogs/referenceselectdialog.h \
    GUI/Dialogs/renamechanneldialog.h \
    GUI/Dialogs/setfileformatdialog.h \
    GUI/Dialogs/setthresholdsdialog.h \
    GUI/Dialogs/spectrogramdialog.h \
    GUI/Dialogs/spikesortingdialog.h \
    GUI/Dialogs/startupdialog.h \
    GUI/Dialogs/stimparamdialog.h \
    GUI/Dialogs/triggerrecorddialog.h \
    GUI/Dialogs/waveformselectdialog.h \
    GUI/Widgets/abstractfigure.h \
    GUI/Widgets/anoutfigure.h \
    GUI/Widgets/controlpanelaudioanalogtab.h \
    GUI/Widgets/controlpanelbandwidthtab.h \
    GUI/Widgets/controlpanelconfiguretab.h \
    GUI/Widgets/controlpanelimpedancetab.h \
    GUI/Widgets/controlpaneltriggertab.h \
    GUI/Widgets/digfigure.h \
    GUI/Widgets/displayedwaveform.h \
    GUI/Widgets/displaylistmanager.h \
    GUI/Widgets/filterdisplayselector.h \
    GUI/Widgets/filterplot.h \
    GUI/Widgets/impedancegradient.h \
    GUI/Widgets/isiplot.h \
    GUI/Widgets/multicolumndisplay.h \
    GUI/Widgets/multiwaveformplot.h \
    GUI/Widgets/pageview.h \
    GUI/Widgets/plotutilities.h \
    GUI/Widgets/psthplot.h \
    GUI/Widgets/scrollbar.h \
    GUI/Widgets/smartspinbox.h \
    GUI/Widgets/spikegradient.h \
    GUI/Widgets/spikeplot.h \
    GUI/Widgets/statusbars.h \
    GUI/Widgets/stimfigure.h \
    GUI/Widgets/tcpdisplay.h \
    GUI/Widgets/voltagespinbox.h \
    GUI/Widgets/waveformdisplaycolumn.h \
    GUI/Widgets/waveformdisplaymanager.h \
    GUI/Windows/controlwindow.h \
    GUI/Windows/probemapwindow.h \
    GUI/Dialogs/impedancefreqdialog.h \
    GUI/Widgets/controlpanel.h \
    GUI/Widgets/spectrogramplot.h \
    GUI/Windows/viewfilterswindow.h

RESOURCES += \
    IntanRHX.qrc

DISTFILES += kernel.cl

INCLUDEPATH += $$PWD/includes/

# Windows
win32: {
LIBS += -L$$PWD/libraries/Windows/ -lOpenCL # OpenCL library
LIBS += -L$$PWD/libraries/Windows/ -lokFrontPanel # Opal Kelly Front Panel library
LIBS += -L$$PWD/libraries/Windows/ -ldelayimp # Microsoft's Delay Import library
QMAKE_LFLAGS += /DELAYLOAD:okFrontPanel.dll # Use delayimp to only load okFrontPanel.dll when necessary,
                                            # so we can give an error message when okFrontPanel.dll is missing
}

# Mac
mac: {
LIBS += -framework OpenCL # Mac OS X built-in OpenCL library
LIBS += -L$$PWD/libraries/Mac/ -lokFrontPanel # Opal Kelly Front Panel library
}

# Linux
unix:!macx: {
LIBS += -lOpenCL
LIBS += -L"$$_PRO_FILE_PWD_/libraries/Linux/" -lokFrontPanel # Opal Kelly Front Panel library
QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\'' # Flag that at runtime, look for shared libraries (like
                                           # libokFrontPanel.so) at the same directory as the binary
}
