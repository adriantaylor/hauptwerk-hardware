#include "Control_Surface_Class.hpp"
#include <AH/Debug/Debug.hpp>
#include <AH/Hardware/ExtendedInputOutput/ExtendedIOElement.hpp>
#include <AH/Hardware/FilteredAnalog.hpp>
#include <MIDI_Constants/Control_Change.hpp>
#include <MIDI_Inputs/MIDIInputElement.hpp>
#include <MIDI_Interfaces/DebugMIDI_Interface.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>
#include <Selectors/Selector.hpp>

#include <AH/Arduino-Wrapper.h>

BEGIN_CS_NAMESPACE

using AH::ExtendedIOElement;

Control_Surface_ &Control_Surface_::getInstance() {
    static Control_Surface_ instance;
    return instance;
}

void Control_Surface_::begin() {
#if defined(ARDUINO) && defined(DEBUG_OUT)
    DEBUG_OUT.begin(AH::defaultBaudRate);
    delay(250);
#endif

    connectDefaultMIDI_Interface();

    FilteredAnalog<>::setupADC();
    ExtendedIOElement::beginAll();
    Updatable<MIDI_Interface>::beginAll();
    MIDIOutputOnly::beginAll();
    beginDisplays();
    MIDIInputElementNote::beginAll();
    MIDIInputElementKP::beginAll();
    MIDIInputElementCC::beginAll();
    MIDIInputElementPC::beginAll();
    MIDIInputElementCP::beginAll();
    MIDIInputElementPB::beginAll();
    MIDIInputElementSysEx::beginAll();
    Updatable<>::beginAll();
    Updatable<Display>::beginAll();
    displayTimer.begin();
}

bool Control_Surface_::connectDefaultMIDI_Interface() {
#if !DISABLE_PIPES
    if (hasSinkPipe() || hasSourcePipe())
        return false;
    auto def = MIDI_Interface::getDefault();
    if (def == nullptr) {
        FATAL_ERROR(F("No default MIDI Interface"), 0xF123);
        return false;
    }
    *this << inpipe << *def;
    *this >> outpipe >> *def;
    return true;
#else
    return MIDI_Interface::getDefault();
#endif
}

void Control_Surface_::disconnectMIDI_Interfaces() {
#if !DISABLE_PIPES
    disconnectSinkPipes();
    disconnectSourcePipes();
#endif
}

void Control_Surface_::loop() {
    ExtendedIOElement::updateAllBufferedInputs();
    Updatable<>::updateAll();
    updateMidiInput();
    updateInputs();
    if (displayTimer)
        updateDisplays();
    ExtendedIOElement::updateAllBufferedOutputs();
}

void Control_Surface_::updateMidiInput() {
#if !DISABLE_PIPES
    Updatable<MIDI_Interface>::updateAll();
#else
    if (auto iface = MIDI_Interface::getDefault()) {
        MIDIReadEvent event = iface->read();
        while (event != MIDIReadEvent::NO_MESSAGE) {
            MIDI_Interface::dispatchIncoming(iface, event);
            switch (event) {
                case MIDIReadEvent::CHANNEL_MESSAGE:
                    sinkMIDIfromPipe(iface->getChannelMessage());
                    break;
                case MIDIReadEvent::SYSEX_CHUNK: // fallthrough
                case MIDIReadEvent::SYSEX_MESSAGE:
                    sinkMIDIfromPipe(iface->getSysExMessage());
                    break;
                case MIDIReadEvent::SYSCOMMON_MESSAGE:
                    sinkMIDIfromPipe(iface->getSysCommonMessage());
                    break;
                case MIDIReadEvent::REALTIME_MESSAGE:
                    sinkMIDIfromPipe(iface->getRealTimeMessage());
                    break;
                case MIDIReadEvent::NO_MESSAGE: break; // LCOV_EXCL_LINE
                default: break;                        // LCOV_EXCL_LINE
            }
            event = iface->read();
        }
    }
#endif
}

#if !DISABLE_PIPES
void Control_Surface_::sendChannelMessageImpl(ChannelMessage msg) {
    this->sourceMIDItoPipe(msg);
}
void Control_Surface_::sendSysExImpl(SysExMessage msg) {
    this->sourceMIDItoPipe(msg);
}
void Control_Surface_::sendSysCommonImpl(SysCommonMessage msg) {
    this->sourceMIDItoPipe(msg);
}
void Control_Surface_::sendRealTimeImpl(RealTimeMessage msg) {
    this->sourceMIDItoPipe(msg);
}
#else
void Control_Surface_::sendChannelMessageImpl(ChannelMessage msg) {
    if (auto def = MIDI_Interface::getDefault())
        def->send(msg);
}
void Control_Surface_::sendSysExImpl(SysExMessage msg) {
    if (auto def = MIDI_Interface::getDefault())
        def->send(msg);
}
void Control_Surface_::sendSysCommonImpl(SysCommonMessage msg) {
    if (auto def = MIDI_Interface::getDefault())
        def->send(msg);
}
void Control_Surface_::sendRealTimeImpl(RealTimeMessage msg) {
    if (auto def = MIDI_Interface::getDefault())
        def->send(msg);
}
#endif

void Control_Surface_::sinkMIDIfromPipe(ChannelMessage midimsg) {
#ifdef DEBUG_MIDI_PACKETS
    if (midimsg.hasTwoDataBytes())
        DEBUG(">>> " << hex << midimsg.header << ' ' << midimsg.data1 << ' '
                     << midimsg.data2 << " (" << midimsg.cable.getOneBased()
                     << ')' << dec);
    else
        DEBUG(">>> " << hex << midimsg.header << ' ' << midimsg.data1 << " ("
                     << midimsg.cable.getOneBased() << ')' << dec);
#endif

    // If the Channel Message callback exists, call it to see if we have to
    // continue handling it.
    if (channelMessageCallback && channelMessageCallback(midimsg))
        return;

    if (midimsg.getMessageType() == MIDIMessageType::ControlChange &&
        midimsg.getData1() == MIDI_CC::Reset_All_Controllers) {
        // Reset All Controllers
        DEBUG(F("Reset All Controllers"));
        MIDIInputElementCC::resetAll();
        MIDIInputElementCP::resetAll();
    } else if (midimsg.getMessageType() == MIDIMessageType::ControlChange &&
               midimsg.getData1() == MIDI_CC::All_Notes_Off) {
        // All Notes Off
        MIDIInputElementNote::resetAll();
    } else {
        switch (midimsg.getMessageType()) {
            case MIDIMessageType::None: break;
            case MIDIMessageType::NoteOff: // fallthrough
            case MIDIMessageType::NoteOn:
                DEBUGFN(F("Updating Note elements with new MIDI "
                          "message."));
                MIDIInputElementNote::updateAllWith(midimsg);
                break;
            case MIDIMessageType::KeyPressure:
                DEBUGFN(F("Updating Key Pressure elements with new MIDI "
                          "message."));
                MIDIInputElementKP::updateAllWith(midimsg);
                break;
            case MIDIMessageType::ControlChange:
                DEBUGFN(F("Updating CC elements with new MIDI "
                          "message."));
                MIDIInputElementCC::updateAllWith(midimsg);
                break;
            case MIDIMessageType::ProgramChange:
                DEBUGFN(F("Updating Program Change elements with new MIDI "
                          "message."));
                MIDIInputElementPC::updateAllWith(midimsg);
                break;
            case MIDIMessageType::ChannelPressure:
                DEBUGFN(F("Updating Channel Pressure elements with new MIDI "
                          "message."));
                MIDIInputElementCP::updateAllWith(midimsg);
                break;
            case MIDIMessageType::PitchBend:
                // Channel Pressure
                DEBUGFN(F("Updating Pitch Bend elements with new MIDI "
                          "message."));
                MIDIInputElementPB::updateAllWith(midimsg);
                break;

            // These MIDI types are not channel messages, so aren't handled here
            // LCOV_EXCL_START
            case MIDIMessageType::SysExStart: break;
            case MIDIMessageType::MTCQuarterFrame: break;
            case MIDIMessageType::SongPositionPointer: break;
            case MIDIMessageType::SongSelect: break;
            case MIDIMessageType::UndefinedSysCommon1: break;
            case MIDIMessageType::UndefinedSysCommon2: break;
            case MIDIMessageType::TuneRequest: break;
            case MIDIMessageType::SysExEnd: break;
            case MIDIMessageType::TimingClock: break;
            case MIDIMessageType::UndefinedRealTime1: break;
            case MIDIMessageType::Start: break;
            case MIDIMessageType::Continue: break;
            case MIDIMessageType::Stop: break;
            case MIDIMessageType::UndefinedRealTime2: break;
            case MIDIMessageType::ActiveSensing: break;
            case MIDIMessageType::SystemReset: break;
            default:
                break;
                // LCOV_EXCL_STOP
        }
    }
}

void Control_Surface_::sinkMIDIfromPipe(SysExMessage msg) {
#ifdef DEBUG_MIDI_PACKETS
    const uint8_t *data = msg.data;
    size_t len = msg.length;
    DEBUG_OUT << ">>> " << hex;
    for (size_t i = 0; i < len; i++)
        DEBUG_OUT << data[i] << ' ';
    DEBUG_OUT << " (" << msg.cable << ')' << dec << endl;
#endif
    // If the SysEx Message callback exists, call it to see if we have to
    // continue handling it.
    if (sysExMessageCallback && sysExMessageCallback(msg))
        return;
    MIDIInputElementSysEx::updateAllWith(msg);
}

void Control_Surface_::sinkMIDIfromPipe(SysCommonMessage msg) {
#ifdef DEBUG_MIDI_PACKETS
    DEBUG_OUT << ">>> " << hex << msg.getMessageType() << ' ' << msg.getData1()
              << ' ' << msg.getData2() << " (" << msg.cable << ')' << dec
              << endl;
#endif
    // If the SysEx Message callback exists, call it to see if we have to
    // continue handling it.
    if (sysCommonMessageCallback && sysCommonMessageCallback(msg))
        return;
}

void Control_Surface_::sinkMIDIfromPipe(RealTimeMessage rtMessage) {
#ifdef DEBUG_MIDI_PACKETS
    DEBUG(">>> " << hex << rtMessage.message << " ("
                 << rtMessage.cable.getOneBased() << ')' << dec);
#endif

    // If the Real-Time Message callback exists, call it to see if we have to
    // continue handling it.
    if (realTimeMessageCallback && realTimeMessageCallback(rtMessage))
        return;
}

void Control_Surface_::updateInputs() {
    MIDIInputElementNote::updateAll();
    MIDIInputElementKP::updateAll();
    MIDIInputElementCC::updateAll();
    MIDIInputElementPC::updateAll();
    MIDIInputElementCP::updateAll();
    MIDIInputElementPB::updateAll();
    MIDIInputElementSysEx::updateAll();
}

void Control_Surface_::beginDisplays() {
    auto &allElements = DisplayElement::getAll();
    auto it = allElements.begin();
    auto end = allElements.end();
    if (it == end)
        return;
    auto previousDisplay = &it->getDisplay();
    // Loop over all display elements
    while (true) {
        ++it;
        // If this is the first element on another display
        if (it == end || &it->getDisplay() != previousDisplay) {
            // Initialize the display
            previousDisplay->begin();
            if (it == end)
                break;
            previousDisplay = &it->getDisplay();
        }
    }
}

void Control_Surface_::updateDisplays() {
    auto &allElements = DisplayElement::getAll();
    auto it = allElements.begin();
    auto end = allElements.end();
    if (it == end)
        return;
    auto prevIt = it;
    auto previousDisplay = &prevIt->getDisplay();
    bool dirty = false;
    // Loop over all display elements
    while (true) {
        dirty |= it->getDirty();
        ++it;
        // If this is the first element on another display
        if (it == end || &it->getDisplay() != previousDisplay) {
            // If there was at least one element on the previous display that
            // has to be redrawn
            if (dirty) {
                // Clear the display
                previousDisplay->clearAndDrawBackground();
                // Update all elements on that display
                for (auto drawIt = prevIt; drawIt != it; ++drawIt)
                    drawIt->draw();
                // Write the buffer to the display
                previousDisplay->display();
            }
            if (it == end)
                break;
            prevIt = it;
            previousDisplay = &it->getDisplay();
            dirty = false;
        }
    }
}

#if CS_TRUE_CONTROL_SURFACE_INSTANCE || defined(DOXYGEN)
Control_Surface_ &Control_Surface = Control_Surface_::getInstance();
#endif

END_CS_NAMESPACE