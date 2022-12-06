
#pragma once
#include <JuceHeader.h>
#include "Source/MainComponent.h"



//==============================================================================
/** As the name suggest, this class does the actual audio processing. */
class MainComponentPlugin  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MainComponentPlugin() {}

    ~MainComponentPlugin() {}

    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        // Only mono/stereo and input/output must have same layout
        const auto& mainOutput = layouts.getMainOutputChannelSet();
        const auto& mainInput  = layouts.getMainInputChannelSet();

        // input and output layout must either be the same or the input must be disabled altogether
        if (! mainInput.isDisabled() && mainInput != mainOutput)
            return false;

        // only allow stereo and mono
        if (mainOutput.size() > 2)
            return false;

        return true;
    }

    void prepareToPlay (double newSampleRate, int samplesPerBlock) override
    {
        main.prepareToPlay(samplesPerBlock,newSampleRate);
    }

    void releaseResources() override
    {
        main.releaseResources();
    }

    void reset() override
    {
        main.reset();
    }

    //==============================================================================
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            main.visualizeInput(buffer.getReadPointer(channel), buffer.getNumSamples());
        }

        main.engine->processBlockSimulation((juce::dsp::AudioBlock<float>(buffer)));

        for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            main.visualizeOutput(buffer.getReadPointer(channel), buffer.getNumSamples());
        }
    }

    void processBlock (juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages) override
    {
        /*for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* ptr = buffer.getReadPointer(channel);
            visualizeInput((float *)ptr, buffer.getNumSamples());
        }

        engine->processBlockSimulation((juce::dsp::AudioBlock<float>(buffer)));

        for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* ptr = buffer.getReadPointer(channel);
            visualizeOutput((float*)ptr, buffer.getNumSamples());
        }*/
    }

    //==============================================================================
    bool hasEditor() const override                                   { return true; }

    juce::AudioProcessorEditor* createEditor() override { 
        return main.editor;
    }

    //==============================================================================
    const juce::String getName() const override                       { return "AudioPluginDemo"; }
    bool acceptsMidi() const override                                 { return false; }
    bool producesMidi() const override                                { return false; }
    double getTailLengthSeconds() const override                      { return 0.0; }

    //==============================================================================
    int getNumPrograms() override                                     { return 0; }
    int getCurrentProgram() override                                  { return 0; }
    void setCurrentProgram (int) override                             {}
    const juce::String getProgramName (int) override                        { return "None"; }
    void changeProgramName (int, const juce::String&) override              {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override
    {
        //// Store an xml representation of our state.
        //if (auto xmlState = state.copyState().createXml())
        //    copyXmlToBinary (*xmlState, destData);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        // Restore our plug-in's state from the xml representation stored in the above
        // method.
        //if (auto xmlState = getXmlFromBinary (data, sizeInBytes))
        //    state.replaceState (ValueTree::fromXml (*xmlState));
    }

    //==============================================================================
    void updateTrackProperties (const TrackProperties& properties) override
    {
       /* {
            const juce::ScopedLock sl (trackPropertiesLock);
            trackProperties = properties;
        }

        juce::MessageManager::callAsync ([this]
        {
            if (auto* editor = dynamic_cast<MainComponentPluginEditor*> (getActiveEditor()))
                 editor->updateTrackProperties();
        });*/
    }

    //TrackProperties getTrackProperties() const
    //{
    //    const juce::ScopedLock sl (trackPropertiesLock);
    //    return trackProperties;
    //}

private:
    //==============================================================================
    /** This is the editor component that our filter will display. */

    static BusesProperties getBusesProperties()
    {
        return BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                                .withOutput ("Output", juce::AudioChannelSet::stereo(), true);
    }

    MainComponent main;

    //juce::CriticalSection trackPropertiesLock;
    //juce::TrackProperties trackProperties;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponentPlugin)
};
