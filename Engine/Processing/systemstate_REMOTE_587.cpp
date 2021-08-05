#include <iostream>
#include "systemstate.h"
#include "signalsources.h"

// Restrict functions for StateItem objects
bool RestrictAlways(const SystemState*) { return true; }
bool RestrictIfRunning(const SystemState* state) { return state->running; }
bool RestrictIfRecording(const SystemState* state) { return state->recording || state->triggerSet; }
bool RestrictIfNotStimController(const SystemState* state) { return state->controllerType != ControllerStimRecordUSB2; }
bool RestrictIfNotStimControllerOrRunning(const SystemState* state)
    { return state->controllerType != ControllerStimRecordUSB2 || state->running; }

SystemState::SystemState(AbstractRHXController* controller_, StimStepSize stimStepSize_, int numSPIPorts_, bool expanderConnected_) :
    controllerType(controller_->getType()),
    stimStepSize(stimStepSize_),
    numSPIPorts(numSPIPorts_),
    expanderConnected(expanderConnected_)
{
    // Private variables
    holdMode = false;
    updateNeeded = false;

    // Intrinsic variables that shouldn't be changed solely through software (e.g. hardware-related, or set in software upon startup)
    signalSources = new SignalSources(this);
    synthetic = new BooleanItem("Synthetic", this, controller_->isSynthetic());
    synthetic->setRestricted(RestrictAlways, "cannot be changed through TCP commands once software has started.");
    playback = new BooleanItem("Playback", this, controller_->isPlayback());
    playback->setRestricted(RestrictAlways, "cannot be changed through TCP commands once software has started.");

    // Streaming data from the board
    running = false;
    manualDelayEnabled.resize(numSPIPorts);
    manualDelayEnabled.fill(false);
    manualDelay.resize(numSPIPorts);
    manualDelay.fill(0);
//    cableLengthPortA = 1.0; // not currently used
//    cableLengthPortB = 1.0;
//    cableLengthPortC = 1.0;
//    cableLengthPortD = 1.0;
//    cableLengthPortE = 1.0;
//    cableLengthPortF = 1.0;
//    cableLengthPortG = 1.0;
//    cableLengthPortH = 1.0;

    int numDigitalInputs = AbstractRHXController::numDigitalIO(controllerType, expanderConnected);
    int numAnalogInputs = AbstractRHXController::numAnalogIO(controllerType, expanderConnected);

    // Saving data
    fileFormat = new DiscreteItemList("FileFormat", this);
    fileFormat->setRestricted(RestrictIfRunning, "cannot be set while board is running");
    fileFormat->addItem("traditional", "Traditional");
    fileFormat->addItem("onefilepersignaltype", "OneFilePerSignalType");
    fileFormat->addItem("onefileperchannel", "OneFilePerChannel");

    saveDigitalOutputs = new BooleanItem("SaveDigitalOutputs", this, true); // TEMP, DEBUG
    saveDigitalOutputs->setRestricted(RestrictIfRunning, "cannot be set while board is running");

    saveDCAmplifierWaveforms = new BooleanItem("SaveDCAmplifierWaveforms", this, false);
    saveDCAmplifierWaveforms->setRestricted(RestrictIfNotStimControllerOrRunning, "cannot be set with a non-Stim board, or while board is running");
    newSaveFilePeriodMinutes = new IntRangeItem("NewSaveFilePeriodMinutes", this, 1, 999, 1);
    newSaveFilePeriodMinutes->setRestricted(RestrictIfRunning, "cannot be set while board is running");

    filename = new StateFilenameItem("Filename", this);
    recording = false;
    triggerSet = false;
    triggered = false;
    sweeping = false;

    triggerSource = new DiscreteItemList("TriggerSource", this);
    triggerSource->setRestricted(RestrictIfRunning, "cannot be set while board is running");
    for (int i = 0; i < numDigitalInputs; ++i) {
        QString name = QString::fromStdString(AbstractRHXController::getDigitalInputChannelName(controllerType, i));
        triggerSource->addItem(name, name, i);
    }
    for (int i = 0; i < numAnalogInputs; ++i) {
        QString name = QString::fromStdString(AbstractRHXController::getAnalogInputChannelName(controllerType, i));
        triggerSource->addItem(name, name, i);
    }
    triggerSource->setIndex(0);

    triggerPolarity = new DiscreteItemList("TriggerPolarity", this);
    triggerPolarity->setRestricted(RestrictIfRunning, "cannot be set while board is running");
    triggerPolarity->addItem("high", "Trigger on Logic High");
    triggerPolarity->addItem("low", "Trigger on Logic Low");
    triggerPolarity->setValue("high");

    preTriggerBuffer = new IntRangeItem("PreTriggerBufferSeconds", this, 1, 30, 1);
    preTriggerBuffer->setRestricted(RestrictIfRunning, "cannot be set while board is running");

    postTriggerBuffer = new IntRangeItem("PostTriggerBufferSeconds", this, 1, 9999, 1);
    postTriggerBuffer->setRestricted(RestrictIfRunning, "cannot be set while board is running");

    saveTriggerSource = new BooleanItem("TriggerSave", this, true);
    saveTriggerSource->setRestricted(RestrictIfRunning, "cannot be set while board is running");

    // Audio
    audioEnabled = new BooleanItem("AudioEnabled", this, false);
    audioVolume = new IntRangeItem("AudioVolume", this, 0, 100, 50);
    audioThreshold = new IntRangeItem("AudioThreshold", this, 0, 200, 0);

    // TCP communication
    globalTcpDataOutputEnabled = new BooleanItem("TCPDataOutputEnabled", this, false);
    tcpCommandCommunicator = new TCPCommunicator();
    tcpDataCommunicator = new TCPCommunicator();

    // Impedance testing
    impedanceFreqValid = false;
    desiredImpedanceFreq = 1000;
    actualImpedanceFreq = 1000;

    // Filtering
    // actualDspCutoffFreq calculated in ControllerInterface
    dspEnabled = true;
    // actualLowerBandwidth calculated in ControllerInterface
    // actualUpperBandwidth calculated in ControllerInterface
    desiredDspCutoffFreq = 1.0;
    // actualLowerSettleBandwidth calculated in ControllerInterface
    desiredLowerBandwidth = 0.1;
    desiredUpperBandwidth = 7500.0;
    highpassFilterFrequency = 250.0;

    notchFreq = new DiscreteItemList("NotchFilterFrequency", this);
    notchFreq->addItem("None", "None", -1.0);
    notchFreq->addItem("50", "50 Hz", 50.0);
    notchFreq->addItem("60", "60 Hz", 60.0);
    notchFreq->setRestricted(RestrictIfRecording, "cannot be set while board is recording");

    lowOrder = new IntRangeItem("LowpassFilterOrder", this, 1,  8, 2);
    lowOrder->setRestricted(RestrictIfRecording, "cannot be set while board is recording");
    lowType = new DiscreteItemList("LowpassFilterType", this);
    lowType->addItem("bessel", "Bessel");
    lowType->addItem("butterworth", "Butterworth");
    lowType->setValue("bessel");
    lowType->setRestricted(RestrictIfRecording, "cannot be set while board is recording");
    lowSWCutoffFreq = new DoubleRangeItem("LowpassFilterCutoff", this, 1.0, 5000.0, 250.0);
    lowSWCutoffFreq->setRestricted(RestrictIfRecording, "cannot be set while board is recording");
    highOrder = new IntRangeItem("HighpassFilterOrder", this, 1, 8, 2);
    highOrder->setRestricted(RestrictIfRecording, "cannot be set while board is recording");
    highType = new DiscreteItemList("HighpassFilterType", this);
    highType->addItem("bessel", "Bessel");
    highType->addItem("butterworth", "Butterworth");
    highType->setValue("bessel");
    highType->setRestricted(RestrictIfRecording, "cannot be set while board is recording");
    highSWCutoffFreq = new DoubleRangeItem("HighpassFilterCutoff", this, 1.0, 5000.0, 250.0);
    highSWCutoffFreq->setRestricted(RestrictIfRecording, "cannot be set while board is recording");

    // Display options
    filterDisplay1 = new DiscreteItemList("FilterDisplay1", this);
    filterDisplay1->addItem("none", "NONE", 0);
    filterDisplay1->addItem("wide", "WIDE", 1);
    filterDisplay1->addItem("low", "LOW", 2);
    filterDisplay1->addItem("high", "HIGH", 3);
    filterDisplay1->addItem("spk", "SPK", 4);
    filterDisplay1->addItem("dc", "DC", 5);
    filterDisplay1->setValue("wide");

    filterDisplay2 = new DiscreteItemList("FilterDisplay2", this);
    filterDisplay2->addItem("none", "NONE", 0);
    filterDisplay2->addItem("wide", "WIDE", 1);
    filterDisplay2->addItem("low", "LOW", 2);
    filterDisplay2->addItem("high", "HIGH", 3);
    filterDisplay2->addItem("spk", "SPK", 4);
    filterDisplay2->addItem("dc", "DC", 5);
    filterDisplay2->setValue("none");

    filterDisplay3 = new DiscreteItemList("FilterDisplay3", this);
    filterDisplay3->addItem("none", "NONE", 0);
    filterDisplay3->addItem("wide", "WIDE", 1);
    filterDisplay3->addItem("low", "LOW", 2);
    filterDisplay3->addItem("high", "HIGH", 3);
    filterDisplay3->addItem("spk", "SPK", 4);
    filterDisplay3->addItem("dc", "DC", 5);
    filterDisplay3->setValue("none");

    filterDisplay4 = new DiscreteItemList("FilterDisplay4", this);
    filterDisplay4->addItem("none", "NONE", 0);
    filterDisplay4->addItem("wide", "WIDE", 1);
    filterDisplay4->addItem("low", "LOW", 2);
    filterDisplay4->addItem("high", "HIGH", 3);
    filterDisplay4->addItem("spk", "SPK", 4);
    filterDisplay4->addItem("dc", "DC", 5);
    filterDisplay4->setValue("none");

    arrangeBy = new DiscreteItemList("ArrangeBy", this);
    arrangeBy->addItem("channel", "Arrange by Channel", 0);
    arrangeBy->addItem("filter", "Arrange by Filter", 0);
    arrangeBy->setValue("channel");

    showDisabledChannels = new BooleanItem("ShowDisabledChannels", this, true);
    showAuxInputs = new BooleanItem("ShowAuxInputs", this, true);
    showSupplyVoltages = new BooleanItem("ShowSupplyVoltages", this, false);

    // Plotting waveforms
    rollMode = new BooleanItem("RollMode", this, true);
    clipWaveforms = new BooleanItem("ClipWaveforms", this, false);
    tScale = new DiscreteItemList("TimeScaleMilliSeconds", this);
    tScale->addItem("10", "10 ms", 10.0);
    tScale->addItem("20", "20 ms", 20.0);
    tScale->addItem("40", "40 ms", 40.0);
    tScale->addItem("100", "100 ms", 100.0);
    tScale->addItem("200", "200 ms", 200.0);
    tScale->addItem("400", "400 ms", 400.0);
    tScale->addItem("1000", "1000 ms", 1000.0);
    tScale->addItem("2000", "2000 ms", 2000.0);
    tScale->addItem("4000", "4000 ms", 4000.0);
    tScale->setValue("2000");

    yScaleWide = new DiscreteItemList("WideScaleMicroVolts", this);
    yScaleWide->addItem("50", "50 " + MuSymbol + "V", 50.0 );
    yScaleWide->addItem("100", "100 " + MuSymbol + "V", 100.0 );
    yScaleWide->addItem("200", "200 " + MuSymbol + "V", 200.0 );
    yScaleWide->addItem("500", "500 " + MuSymbol + "V", 500.0 );
    yScaleWide->addItem("1000", "1000 " + MuSymbol + "V", 1000.0 );
    yScaleWide->addItem("2000", "2000 " + MuSymbol + "V", 2000.0 );
    yScaleWide->addItem("5000", "5000 " + MuSymbol + "V", 5000.0 );
    yScaleWide->setValue("500");

    yScaleLow = new DiscreteItemList("LowScaleMicroVolts", this);
    yScaleLow->addItem("50", "50 " + MuSymbol + "V", 50.0 );
    yScaleLow->addItem("100", "100 " + MuSymbol + "V", 100.0 );
    yScaleLow->addItem("200", "200 " + MuSymbol + "V", 200.0 );
    yScaleLow->addItem("500", "500 " + MuSymbol + "V", 500.0 );
    yScaleLow->addItem("1000", "1000 " + MuSymbol + "V", 1000.0 );
    yScaleLow->addItem("2000", "2000 " + MuSymbol + "V", 2000.0 );
    yScaleLow->addItem("5000", "5000 " + MuSymbol + "V", 5000.0 );
    yScaleLow->setValue("500");

    yScaleHigh = new DiscreteItemList("HighScaleMicroVolts", this);
    yScaleHigh->addItem("50", "50 " + MuSymbol + "V", 50.0 );
    yScaleHigh->addItem("100", "100 " + MuSymbol + "V", 100.0 );
    yScaleHigh->addItem("200", "200 " + MuSymbol + "V", 200.0 );
    yScaleHigh->addItem("500", "500 " + MuSymbol + "V", 500.0 );
    yScaleHigh->addItem("1000", "1000 " + MuSymbol + "V", 1000.0 );
    yScaleHigh->addItem("2000", "2000 " + MuSymbol + "V", 2000.0 );
    yScaleHigh->addItem("5000", "5000 " + MuSymbol + "V", 5000.0 );
    yScaleHigh->setValue("500");

    yScaleAux = new DiscreteItemList("AuxScaleVolts", this);
    yScaleAux->addItem("0.1", "0.1 V", 0.1);
    yScaleAux->addItem("0.2", "0.2 V", 0.2);
    yScaleAux->addItem("0.5", "0.5 V", 0.5);
    yScaleAux->addItem("1.0", "1 V", 1.0);
    yScaleAux->addAlternateValueName("1.0", "1");
    yScaleAux->addItem("2.0", "2 V", 2.0);
    yScaleAux->addAlternateValueName("2.0", "2");
    yScaleAux->setValue("1.0");

    yScaleAnalog = new DiscreteItemList("AnalogScaleVolts", this);
    yScaleAnalog->addItem("0.1", "0.1 V", 0.1);
    yScaleAnalog->addItem("0.2", "0.2 V", 0.2);
    yScaleAnalog->addItem("0.5", "0.5 V", 0.5);
    yScaleAnalog->addItem("1.0", "1 V", 1.0);
    yScaleAnalog->addAlternateValueName("1.0", "1");
    yScaleAnalog->addItem("2.0", "2 V", 2.0);
    yScaleAnalog->addAlternateValueName("2.0", "2");
    yScaleAnalog->addItem("5.0", "5 V", 5.0);
    yScaleAnalog->addAlternateValueName("5.0", "5");
    yScaleAnalog->addItem("10.0", "10 V", 10.0);
    yScaleAnalog->addAlternateValueName("10.0", "10");
    yScaleAnalog->setValue("1.0");

    yScaleDC = new DiscreteItemList("DCScaleVolts", this);
    yScaleDC->addItem("0.5", "0.5 V", 0.5);
    yScaleDC->addItem("1.0", "1 V", 1.0);
    yScaleDC->addAlternateValueName("1.0", "1");
    yScaleDC->addItem("2.0", "2 V", 2.0);
    yScaleDC->addAlternateValueName("2.0", "2");
    yScaleDC->addItem("5.0", "5 V", 5.0);
    yScaleDC->addAlternateValueName("5.0", "5");
    yScaleDC->addItem("10.0", "10 V", 10.0);
    yScaleDC->addAlternateValueName("10.0", "10");
    yScaleDC->setValue("1.0");
    yScaleDC->setRestricted(RestrictIfNotStimController, "cannot be set with a non-Stim controller");

    // ISI
    isiChannel = new ChannelNameItem("ISIChannel", this, "N/A");

    tSpanISI = new DiscreteItemList("ISITimeSpanMsec", this);
    tSpanISI->addItem("50", "50 ms", 50);
    tSpanISI->addItem("100", "100 ms", 100);
    tSpanISI->addItem("200", "200 ms", 200);
    tSpanISI->addItem("500", "500 ms", 500);
    tSpanISI->addItem("1000", "1000 ms", 1000);
    tSpanISI->setValue("200");

    binSizeISI = new DiscreteItemList("ISIBinSizeMsec", this);
    binSizeISI->addItem("1", "1 ms", 1);
    binSizeISI->addItem("2", "2 ms", 2);
    binSizeISI->addItem("5", "5 ms", 5);
    binSizeISI->addItem("10", "10 ms", 10);
    binSizeISI->addItem("20", "20 ms", 20);
    binSizeISI->setValue("5");

    yAxisLogISI = new BooleanItem("ISIYAxisLog", this, false);

    saveCsvFileISI = new BooleanItem("ISISaveCsvFile", this, true);
    saveMatFileISI = new BooleanItem("ISISaveMatFile", this, true);
    savePngFileISI = new BooleanItem("ISISavePngFile", this, true);

    // PSTH
    psthChannel = new ChannelNameItem("PSTHChannel", this, "N/A");

    tSpanPreTriggerPSTH = new DiscreteItemList("PSTHPreTriggerSpanMsec", this);
    tSpanPreTriggerPSTH->addItem("100", "100 ms", 100);
    tSpanPreTriggerPSTH->addItem("200", "200 ms", 200);
    tSpanPreTriggerPSTH->addItem("500", "500 ms", 500);
    tSpanPreTriggerPSTH->addItem("1000", "1000 ms", 1000);
    tSpanPreTriggerPSTH->addItem("2000", "2000 ms", 2000);
    tSpanPreTriggerPSTH->setValue("50");

    tSpanPostTriggerPSTH = new DiscreteItemList("PSTHPostTriggerSpanMsec", this);
    tSpanPostTriggerPSTH->addItem("100", "100 ms", 100);
    tSpanPostTriggerPSTH->addItem("200", "200 ms", 200);
    tSpanPostTriggerPSTH->addItem("500", "500 ms", 500);
    tSpanPostTriggerPSTH->addItem("1000", "1000 ms", 1000);
    tSpanPostTriggerPSTH->addItem("2000", "2000 ms", 2000);
    tSpanPostTriggerPSTH->setValue("500");

    binSizePSTH = new DiscreteItemList("PSTHBinSizeMsec", this);
    binSizePSTH->addItem("1", "1 ms", 1);
    binSizePSTH->addItem("2", "2 ms", 2);
    binSizePSTH->addItem("5", "5 ms", 5);
    binSizePSTH->addItem("10", "10 ms", 10);
    binSizePSTH->addItem("20", "20 ms", 20);
    binSizePSTH->addItem("50", "50 ms", 50);
    binSizePSTH->addItem("100", "100 ms", 100);
    binSizePSTH->setValue("5");

    maxNumTrialsPSTH = new DiscreteItemList("PSTHMaxNumTrials", this);
    maxNumTrialsPSTH->addItem("10", "10", 10);
    maxNumTrialsPSTH->addItem("20", "20", 20);
    maxNumTrialsPSTH->addItem("50", "50", 50);
    maxNumTrialsPSTH->addItem("100", "100", 100);
    maxNumTrialsPSTH->addItem("200", "200", 200);
    maxNumTrialsPSTH->addItem("500", "500", 500);
    maxNumTrialsPSTH->setValue("50");

    digitalTriggerPSTH = new DiscreteItemList("PSTHDigitalTrigger", this);
    for (int i = 0; i < numDigitalInputs; ++i) {
        QString name = QString::fromStdString(AbstractRHXController::getDigitalInputChannelName(controllerType, i));
        digitalTriggerPSTH->addItem(name, name, i);
    }
    digitalTriggerPSTH->setIndex(0);

    triggerPolarityPSTH = new DiscreteItemList("PSTHTriggerPolarity", this);
    triggerPolarityPSTH->addItem("rising", "Rising Edge", +1);
    triggerPolarityPSTH->addItem("falling", "Falling Edge", -1);
    triggerPolarityPSTH->setValue("rising");

    saveCsvFilePSTH = new BooleanItem("PSTHSaveCsvFile", this, true);
    saveMatFilePSTH = new BooleanItem("PSTHSaveMatFile", this, true);
    savePngFilePSTH = new BooleanItem("PSTHSavePngFile", this, true);

    // Spectrogram
    spectrogramChannel = new ChannelNameItem("SpectrogramChannel", this, "N/A");
    sampleRate = new DiscreteItemList("SampleRate", this);  // TODO: move this out of Spectrogram area
    if (controllerType != ControllerStimRecordUSB2) {

        sampleRate->addItem("1000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate1000Hz)), 1000.0);
        sampleRate->addItem("1250", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate1250Hz)), 1250.0);
        sampleRate->addItem("1500", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate1500Hz)), 1500.0);
        sampleRate->addItem("2000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate2000Hz)), 2000.0);
        sampleRate->addItem("2500", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate2500Hz)), 2500.0);
        sampleRate->addItem("3000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate3000Hz)), 3000.0);
        sampleRate->addItem("3333", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate3333Hz)), 10000.0 / 3.0);
        sampleRate->addItem("4000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate4000Hz)), 4000.0);
        sampleRate->addItem("5000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate5000Hz)), 5000.0);
        sampleRate->addItem("6250", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate6250Hz)), 6250.0);
        sampleRate->addItem("8000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate8000Hz)), 8000.0);
        sampleRate->addItem("10000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate10000Hz)), 10000.0);
        sampleRate->addItem("12500", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate12500Hz)), 12500.0);
        sampleRate->addItem("15000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate15000Hz)), 15000.0);
    }

    sampleRate->addItem("20000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate20000Hz)), 20000.0);
    sampleRate->addItem("25000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate25000Hz)), 25000.0);
    sampleRate->addItem("30000", QString::fromStdString(AbstractRHXController::getSampleRateString(SampleRate30000Hz)), 30000.0);

    sampleRate->setValue(QString::number(round(controller_->getSampleRate())));
    sampleRate->setRestricted(RestrictAlways, "cannot be changed through TCP commands once software has started.");

    displayModeSpectrogram = new DiscreteItemList("SpectrogramDisplayMode", this);
    displayModeSpectrogram->addItem("spectrogram", "Spectrogram", 0);
    displayModeSpectrogram->addItem("spectrum", "Spectrum", 1);
    displayModeSpectrogram->setValue("spectrogram");

    tScaleSpectrogram = new DiscreteItemList("SpectrogramTimeScaleSeconds", this);
    tScaleSpectrogram->addItem("2", "2 s", 2.0);
    tScaleSpectrogram->addItem("5", "5 s", 5.0);
    tScaleSpectrogram->addItem("10", "10 s", 10.0);
    tScaleSpectrogram->setValue("5");

    fftSizeSpectrogram = new DiscreteItemList("SpectrogramFFTSize", this);
    fftSizeSpectrogram->addItem("256", "256", 256);
    fftSizeSpectrogram->addItem("512", "512", 512);
    fftSizeSpectrogram->addItem("1024", "1024", 1024);
    fftSizeSpectrogram->addItem("2048", "2048", 2048);
    fftSizeSpectrogram->addItem("4096", "4096", 4096);
    fftSizeSpectrogram->addItem("8192", "8192", 8192);
    fftSizeSpectrogram->addItem("16384", "16384", 16384);
    fftSizeSpectrogram->setValue("2048");

    int fNyquist = qFloor(sampleRate->getNumericValue() / 2.0);
    fMinSpectrogram = new IntRangeItem("SpectrogramFreqMin", this, 0, fNyquist - 10, 0);
    fMaxSpectrogram = new IntRangeItem("SpectrogramFreqMax", this, 10, fNyquist, qMin(fNyquist, 200));
    fMarkerSpectrogram = new IntRangeItem("SpectrogramFreqMarker", this, 0, fNyquist, 60);
    showFMarkerSpectrogram = new BooleanItem("SpectrogramShowFreqMarker", this, true);
    numHarmonicsSpectrogram = new IntRangeItem("SpectrogramFreqMarkerNumHarmonics", this, 0, 9, 0);

    digitalDisplaySpectrogram = new DiscreteItemList("SpectrogramDigitalDisplay", this);
    digitalDisplaySpectrogram->addItem("none", "none", -1);
    for (int i = 0; i < numDigitalInputs; ++i) {
        QString name = QString::fromStdString(AbstractRHXController::getDigitalInputChannelName(controllerType, i));
        digitalDisplaySpectrogram->addItem(name, name, i);
    }
    digitalDisplaySpectrogram->setValue("none");

    saveCsvFileSpectrogram = new BooleanItem("SpectrogramSaveCsvFile", this, true);
    saveMatFileSpectrogram = new BooleanItem("SpectrogramSaveMatFile", this, true);
    savePngFileSpectrogram = new BooleanItem("SpectrogramSavePngFile", this, true);

    // Spike scope
    spikeScopeChannel = new ChannelNameItem("SpikeScopeChannel", this, "N/A");
    spikeThreshold = new IntRangeVectorItem("SpikeThresholdMicroVolts", this, signalSources->numAmplifierChannels(), -5000, 5000, -70);
    yScaleSpikeScope = new DiscreteItemList("SpikeScopeScaleMicroVolts", this);
    yScaleSpikeScope->addItem("50", "50 " + MuSymbol + "V", 50.0);
    yScaleSpikeScope->addItem("100", "100 " + MuSymbol + "V", 100.0);
    yScaleSpikeScope->addItem("200", "200 " + MuSymbol + "V", 200.0);
    yScaleSpikeScope->addItem("500", "500 " + MuSymbol + "V", 500.0);
    yScaleSpikeScope->addItem("1000", "1000 " + MuSymbol + "V", 1000.0);
    yScaleSpikeScope->addItem("2000", "2000 " + MuSymbol + "V", 2000.0);
    yScaleSpikeScope->addItem("5000", "5000 " + MuSymbol + "V", 5000.0);
    yScaleSpikeScope->setValue("500");
    tScaleSpikeScope = new DiscreteItemList("SpikeScopeTimeScaleMilliSeconds", this);
    tScaleSpikeScope->addItem("2", "2 ms", 2.0);
    tScaleSpikeScope->addItem("4", "4 ms", 4.0);
    tScaleSpikeScope->addItem("6", "6 ms", 6.0);
    tScaleSpikeScope->setValue("2");
    numSpikesDisplayed = new DiscreteItemList("SpikeScopeNumSpikes", this);
    numSpikesDisplayed->addItem("10", "10", 10);
    numSpikesDisplayed->addItem("20", "20", 20);
    numSpikesDisplayed->addItem("30", "30", 30);
    numSpikesDisplayed->addItem("50", "50", 50);
    numSpikesDisplayed->addItem("100", "100", 100);
    numSpikesDisplayed->setValue("20");
    suppressionEnabled = new BooleanItem("ArtifactSuppressionEnabled", this, true);
    artifactsShown = new BooleanItem("ArtifactsShown", this, true);
    suppressionThreshold = new IntRangeItem("ArtifactSuppressionThresholdMicroVolts", this, 0, 5000, 300);

    // Stimulation only
    desiredLowerSettleBandwidth = 1000.0;
    useFastSettle = false;
    headstageGlobalSettle = false;
    fastSettleEnabled = false;
    chargeRecoveryMode = false;
    chargeRecoveryCurrentLimit = RHXRegisters::ChargeRecoveryCurrentLimit::CurrentLimit10nA;
    chargeRecoveryTargetVoltage = 0.0;
    stimParamsHaveChanged = false;
}

SystemState::~SystemState()
{
    delete signalSources;

    for (auto p = stateSingleItems.begin(); p != stateSingleItems.end(); ++p) {
        delete p->second;
    }
    for (auto p = stateVectorItems.begin(); p != stateVectorItems.end(); ++p) {
        delete p->second;
    }
}

AmplifierSampleRate SystemState::getSampleRateEnum()
{
    if (controllerType != ControllerStimRecordUSB2) {
        return (AmplifierSampleRate) sampleRate->getIndex();
    }
    else {
        if (sampleRate->getValue() == "20000")
            return SampleRate20000Hz;
        else if (sampleRate->getValue() == "25000")
            return SampleRate25000Hz;
        else if (sampleRate->getValue() == "30000")
            return SampleRate30000Hz;
        else {
            qDebug() << "Error... getSampleRateEnum() failed for some reason";
            return SampleRate20000Hz;
        }
    }
}

FileFormat SystemState::getFileFormatEnum()
{
    if (fileFormat->getValue() == "traditional")
        return FileFormatIntan;
    else if (fileFormat->getValue() == "onefilepersignaltype")
        return FileFormatFilePerSignalType;
    else if (fileFormat->getValue() == "onefileperchannel")
        return FileFormatFilePerChannel;
    else {
        qDebug() << "Error... getFileFormatEnum() failed for some reason";
        return (FileFormat) -1;
    }
}

StateSingleItem* SystemState::locateStateSingleItem(const QString& parameterName) const
{
    auto p = stateSingleItems.find(parameterName.toStdString());
    if (p == stateSingleItems.end()) {
        return nullptr;
    }
    return p->second;
}

StateVectorItem* SystemState::locateStateVectorItem(const QString& parameterName) const
{
    auto p = stateVectorItems.find(parameterName.toStdString());
    if (p == stateVectorItems.end()) {
        return nullptr;
    }
    return p->second;
}

StateFilenameItem* SystemState::locateStateFilenameItem(const QString& fullParameterName, QString &pathOrBase_) const
{
    // Divide fullParameterName into parameterName and pathOrBase.
    QString parameterName = fullParameterName.section('.', 0, 0);
    QString pathOrBase = fullParameterName.section('.', 1, 1);

    // Find the item, returning nullptr if not found.
    auto p = stateFilenameItems.find(parameterName.toStdString());
    if (p == stateFilenameItems.end()) {
        return nullptr;
    }

    // Return pathOrBase by reference (returning nullptr if pathOrBase isn't path or baseFilename)
    if (pathOrBase.toLower() != p->second->getPathParameterName().toLower() && pathOrBase.toLower() != p->second->getBaseFilenameParameterName().toLower()) {
        return nullptr;
    } else {
        pathOrBase_ = pathOrBase;
        return p->second;
    }
}

// Should be called after 'addAmplifierchannels()', i.e. when ports have been rescanned and channels may be added or removed
void SystemState::updateChannels()
{
    if (spikeThreshold->numberOfItems() == 0) {
        spikeThreshold->resizeVector(signalSources->numAmplifierChannels());
    } else {
        cerr << "Error: Need to implement code in SystemState::updateChannels() to handle rescan intelligently." << endl; // TODO
        // Maybe create a map of channel name ("A-000") to spike threshold parameters, and use this to copy old parameters
        // over when a rescan is performed and some channels have remained while others have been added or removed.
    }
}

int SystemState::getSerialIndex(const QString& channelName) const
{
    if (channelName == "N/A" || channelName.isEmpty()) {
        return -1;
    }
    int channelsPerStream = RHXDataBlock::channelsPerStream(controllerType);
    Channel* thisChannel = signalSources->findChannelFromName(channelName);
    if (thisChannel)
        return thisChannel->getBoardStream() * channelsPerStream + thisChannel->getChipChannel();
    else
        return -1;
}

void SystemState::holdUpdate()
{
    holdMode = true;
    updateNeeded = false;
}

void SystemState::releaseUpdate()
{
    if (updateNeeded) {
        emit stateChanged();
        updateNeeded = false;
    }
    holdMode = false;
}

int SystemState::usedXPUIndex()
{
    if (cpuInfo.used)
        return 0;
    for (int gpu = 0; gpu < gpuList.size(); gpu++) {
        if (gpuList[gpu].used)
            return gpu + 1;
    }
    return -1;
}

// Public (but should only be used by BooleanItem, DiscreteItemList, etc.): All state changes should go through this function.
void SystemState::update()
{
    if (holdMode) {
        updateNeeded = true;
    } else {
        emit stateChanged();
    }
}
