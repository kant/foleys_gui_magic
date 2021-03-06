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

#pragma once

namespace foleys
{

class MagicBuilder;

class ToolBox  : public juce::Component,
                 public juce::DragAndDropContainer,
                 public juce::KeyListener,
                 private juce::Timer
{
public:
    ToolBox (juce::Component* parent, MagicBuilder& builder);
    ~ToolBox();

    void loadDialog();
    void saveDialog();

    void loadGUI (const juce::File& file);
    void saveGUI (const juce::File& file);

    void paint (juce::Graphics& g) override;

    void resized() override;

    void timerCallback () override;

    void setSelectedNode (const juce::ValueTree& node);
    void setNodeToEdit (juce::ValueTree node);

    void stateWasReloaded();

    bool keyPressed (const juce::KeyPress& key) override;
    bool keyPressed (const juce::KeyPress& key, juce::Component* originalComponent) override;

private:

    juce::File getLastLocation() const;
    std::unique_ptr<juce::FileFilter> getFileFilter() const;

    juce::Component::SafePointer<juce::Component> parent;
    juce::Point<int>    parentPos;
    int                 parentHeight = 0;

    MagicBuilder&       builder;
    juce::UndoManager&  undo;

    juce::TextButton    fileMenu   { TRANS ("File...") };
    juce::TextButton    undoButton { TRANS ("Undo") };

    juce::TextButton    editSwitch { TRANS ("Edit") };

    juce::File          lastLocation;

    GUITreeEditor       treeEditor          { builder };
    PropertiesEditor    propertiesEditor    { builder };
    Palette             palette             { builder };

    juce::StretchableLayoutManager    resizeManager;
    juce::StretchableLayoutResizerBar resizer1 { &resizeManager, 1, false };
    juce::StretchableLayoutResizerBar resizer3 { &resizeManager, 3, false };

    juce::TooltipWindow tooltip      { this };

    std::unique_ptr<juce::FileBrowserComponent> fileBrowser;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolBox)
};

} // namespace foleys
