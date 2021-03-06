/*
 ==============================================================================
    Copyright (c) 2019 Foleys Finest Audio Ltd. - Daniel Walz
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
    OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
    OF THE POSSIBILITY OF SUCH DAMAGE.
 ==============================================================================
 */



namespace foleys
{

MagicPluginEditor::MagicPluginEditor (MagicProcessorState& stateToUse, std::unique_ptr<MagicBuilder> builderToUse)
  : juce::AudioProcessorEditor (stateToUse.getProcessor()),
    processorState (stateToUse)
{
#if JUCE_MODULE_AVAILABLE_juce_opengl
    oglContext.attachTo (*this);
#endif

    if (builderToUse.get() != nullptr)
        builder = std::move (builderToUse);
    else
        builder = createBuilderInstance();

    builder->restoreGUI (*this);

    builder->updateAll();

    updateSize();

#if FOLEYS_SHOW_GUI_EDITOR_PALLETTE
    builder->attachToolboxToWindow (*this);
#endif
}

MagicPluginEditor::MagicPluginEditor (MagicProcessorState& stateToUse, const char* data, const int dataSize, std::unique_ptr<MagicBuilder> builderToUse)
  : juce::AudioProcessorEditor (stateToUse.getProcessor()),
    processorState (stateToUse)
{
#if JUCE_MODULE_AVAILABLE_juce_opengl
    oglContext.attachTo (*this);
#endif

    if (builderToUse.get() != nullptr)
        builder = std::move (builderToUse);
    else
        builder = createBuilderInstance();

    setConfigTree (data, dataSize);

    updateSize();

#if FOLEYS_SHOW_GUI_EDITOR_PALLETTE
    builder->attachToolboxToWindow (*this);
#endif
}

MagicPluginEditor::~MagicPluginEditor()
{
#if JUCE_MODULE_AVAILABLE_juce_opengl
    oglContext.detach();
#endif
}

std::unique_ptr<MagicBuilder> MagicPluginEditor::createBuilderInstance()
{
    auto newBuilder = std::make_unique<MagicGUIBuilder<juce::AudioProcessor>>(processorState.getProcessor(), &processorState);
    newBuilder->registerJUCEFactories();
    newBuilder->registerJUCELookAndFeels();

    return newBuilder;
}

void MagicPluginEditor::updateSize()
{
    int width = 600;
    int height = 400;

    setResizable (true, true);

    processorState.getLastEditorSize (width, height);
    setSize (width, height);
}

void MagicPluginEditor::setConfigTree (const juce::ValueTree& gui)
{
    builder->setConfigTree (gui);
    builder->restoreGUI (*this);
}

void MagicPluginEditor::setConfigTree (const char* data, const int dataSize)
{
    juce::String text (data, dataSize);
    auto gui = juce::ValueTree::fromXml (text);
    setConfigTree (gui);
}

void MagicPluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void MagicPluginEditor::resized()
{
    builder->updateLayout();

    processorState.setLastEditorSize (getWidth(), getHeight());
}

} // namespace foleys
