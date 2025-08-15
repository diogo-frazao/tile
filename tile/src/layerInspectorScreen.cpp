#include "screens.h"
#include "imgui.h"
#include "core/app.h"
#include "SDL.h"
#include <cstdint>
#include "core/debugUtils.h"

static int s_spritesIconSize = 50;
static int s_paddingBetweenIcons = 3;
static int s_spritesColumns = 6;

static int s_selectedSpriteIndex = -1;
static int s_selectedTabIndex = -1;
static bool s_didSelectSpriteThisFrame = false;

static bool s_canShowLayerInspector = true;

void LayerInspectorScreen::start()
{
    getMainScreen()._tilePlayground._lastSpriteUndoDelegate = std::bind(&LayerInspectorScreen::onUndoLastPlacedSprite, this);
}

void drawRectToLocateSelectedSprite()
{
    auto& placedSprite = getMainScreen()._tilePlayground._placedSprites[s_selectedSpriteIndex];    

    RectCollider placedSpriteCollider {placedSprite.sprite.position, placedSprite.sprite.size};
    placedSpriteCollider.debugColor = {0, 255, 0, 255};
    drawRect(placedSpriteCollider);
}

void LayerInspectorScreen::render()
{
    if (!s_didSelectSpriteThisFrame)
    {
        return;
    }
    
    if (!isIndexValid(s_selectedSpriteIndex))
    {
        return;
    }

    drawRectToLocateSelectedSprite();
}

void LayerInspectorScreen::onUndoLastPlacedSprite()
{
    s_selectedSpriteIndex = k_invalidIndex;
}

void setAllSpritesVisibleForLayer(bool visible, LayerType layer)
{
    for (size_t i = 0; i < getMainScreen()._tilePlayground._placedSprites.size(); ++i)
    {
        auto& placedSprite = getMainScreen()._tilePlayground._placedSprites[i];
        if(placedSprite.layer != layer)
        {
            continue;
        }

        bool isSpriteInHand = (i == getMainScreen()._tilePlayground._spriteInHandIndex);
        if (isSpriteInHand) 
        {
            continue;
        }

        placedSprite.sprite.isVisible = visible;
    }
}

void createSpritesTreeForLayer(LayerType layer, ImTextureRef atlasImageRef)
{
    s_selectedTabIndex = static_cast<int>(layer);
    ImGui::PushItemWidth(100);
    ImGui::SliderInt("Icon Size", &s_spritesIconSize, 30, 60);
    ImGui::SameLine();
    ImGui::SliderInt("Icons per row", &s_spritesColumns, 2, 8);
    ImGui::PopItemWidth();
    
    ImVec2 spritesTreeSize {ImGui::GetContentRegionAvail().x * 0.6f, 0};
    if (ImGui::BeginChild("##SpritesTreeContainer", spritesTreeSize, ImGuiChildFlags_Borders))
    {   
        if (ImGui::BeginTable("##SpriteTree", s_spritesColumns))
        {
            for (size_t i = 0; i < getMainScreen()._tilePlayground._placedSprites.size(); ++i)
            {
                auto& placedSprite = getMainScreen()._tilePlayground._placedSprites[i];
                if(placedSprite.layer != layer)
                {
                    continue;
                }

                bool isSpriteInHand = (i == getMainScreen()._tilePlayground._spriteInHandIndex);
                if (isSpriteInHand) 
                {
                    continue;
                }

                ImGui::PushID(i);
                ImGui::TableNextColumn();

                ImVec2 mousePosition = ImGui::GetCursorScreenPos();
                ImVec2 boxStartPosition {mousePosition.x, mousePosition.y};
                ImVec2 boxEndPosition {boxStartPosition.x + s_spritesIconSize, boxStartPosition.y + s_spritesIconSize};
                
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                bool didClickBox = ImGui::Button("##i", ImVec2(s_spritesIconSize, s_spritesIconSize));

                if (didClickBox)
                { 
                    s_selectedSpriteIndex = i;
                    s_didSelectSpriteThisFrame = true;
                }

                if(s_selectedSpriteIndex == i)
                {
                    ImU32 selectedColor = ImGui::GetColorU32(IM_COL32(0, 255, 0, 255));
                    drawList->AddRect(boxStartPosition, boxEndPosition, selectedColor, 1.f, 0, 2.f);
                }
                
                ImVec2 spriteStartPosition {boxStartPosition.x + s_paddingBetweenIcons, 
                    boxStartPosition.y + s_paddingBetweenIcons};
                ImVec2 spriteEndPosition {boxEndPosition.x - 
                    s_paddingBetweenIcons, boxEndPosition.y - s_paddingBetweenIcons};

                IVec2 placedSpriteOffset {placedSprite.sprite.offset};

                ImVec2  spriteUV0 {static_cast<float>(placedSpriteOffset.x) / s_atlasTextureWidth, 
                    static_cast<float>(placedSpriteOffset.y) / s_atlasTextureHeight};
                ImVec2 spriteUV1 {static_cast<float>(placedSpriteOffset.x + placedSprite.sprite.size.x) / s_atlasTextureWidth, 
                    static_cast<float>(placedSpriteOffset.y + placedSprite.sprite.size.y) / s_atlasTextureHeight};

                drawList->AddImage(atlasImageRef, spriteStartPosition, spriteEndPosition, spriteUV0, spriteUV1);

                ImGui::PopID();
            }

            ImGui::EndTable();
        }
    }
    
    ImGui::EndChild();
}

static bool imGuiColoredButton(const char* buttonName, int colorIndex)
{
    // Copied from ImGui Demo. 0-Red, 1-Yellow, 2-Green, 3-LightGreen, 4-Blue, 5-Purple, 6-Pink
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(colorIndex / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(colorIndex / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(colorIndex / 7.0f, 0.8f, 0.8f));
    bool didPressButton = ImGui::Button(buttonName);
    ImGui::PopStyleColor(3);

    return didPressButton;
}

static void showResetHistoryHelpMarker()
{
    ImGui::SameLine();
    ImGui::TextDisabled("(!)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted("WARNING: This operation resets the history used for undo/redo sprites.");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void displayPropertiesForSelectedSprite()
{
    if (ImGui::BeginChild("##PropertiesChild"))
    {
        ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, 10);
        if (ImGui::BeginTable("##Properties", 1))
        {
            if(isIndexValid(s_selectedSpriteIndex))
            {
                auto& selectedPlaceableSprite = getMainScreen()._tilePlayground._placedSprites[s_selectedSpriteIndex];
                Sprite& selectedSprite = selectedPlaceableSprite.sprite;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGui::SeparatorText("Object Properties");

                ImGui::PushItemWidth(90);
                ImGui::SliderFloat("X", &selectedSprite.position.x, 0, k_screenWidth);
                ImGui::SameLine();
                ImGui::SliderFloat("Y", &selectedSprite.position.y, 0, k_screenHeight);
                ImGui::PopItemWidth();
                ImGui::Checkbox("Visible", &selectedSprite.isVisible);

                if (imGuiColoredButton("Delete from Scene", 0))
                {
                    getMainScreen()._tilePlayground.deletePlacedSprite(&selectedPlaceableSprite);
                    s_selectedSpriteIndex = k_invalidIndex;
                }
                showResetHistoryHelpMarker();

                ImGui::SeparatorText("Layer Properties");

                if (imGuiColoredButton("Toogle Layer Visibility", 5))
                {
                    setAllSpritesVisibleForLayer(!selectedSprite.isVisible, selectedPlaceableSprite.layer);
                }

                if (ImGui::Button("Bring forward"))
                {
                    size_t newSpriteIndex = 0;
                    bool didSucceed = getMainScreen()._tilePlayground.bringPlacedSpriteForward(s_selectedSpriteIndex, 
                        newSpriteIndex);
                    s_selectedSpriteIndex = didSucceed ? newSpriteIndex : s_selectedSpriteIndex;
                }
                showResetHistoryHelpMarker();
                
                if (ImGui::Button("Send backward"))
                {
                    size_t newSpriteIndex = 0;
                    bool didSucceed = getMainScreen()._tilePlayground.sendPlacedSpriteBackwards(s_selectedSpriteIndex, 
                        newSpriteIndex);
                    s_selectedSpriteIndex = didSucceed ? newSpriteIndex : s_selectedSpriteIndex;
                }
                showResetHistoryHelpMarker();
            }
            
            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
    }

    ImGui::EndChild();
}

void toggleLayerInspectorOnLeaveMainScreen()
{
    bool isOutsideOfMainScreen = getPanelScreen()._isPanelActive;
    if (isOutsideOfMainScreen == s_canShowLayerInspector)
    {
        s_canShowLayerInspector = !isOutsideOfMainScreen;
    }
}


void LayerInspectorScreen::update()
{
    static ImTextureID atlasId = (ImTextureID)(intptr_t)s_atlasTexture;
    D_ASSERT(atlasId != 0, "Invalid texture atlas");
    static ImTextureRef atlasImageRef{atlasId};

    s_didSelectSpriteThisFrame = false;

    toggleLayerInspectorOnLeaveMainScreen();
    if (!s_canShowLayerInspector)
    {
        return;
    }

    ImGui::Begin("Layer Inspector");
    ImGui::BeginTabBar("##Layers");

    int lastSelectedTabIndex = s_selectedTabIndex;

    if (ImGui::BeginTabItem(s_layersAsString[0]))
    {
        createSpritesTreeForLayer(BACKGROUND, atlasImageRef);
        ImGui::SameLine();
        displayPropertiesForSelectedSprite();

        ImGui::EndTabItem();
    }


    if (ImGui::BeginTabItem(s_layersAsString[1]))
    {
        createSpritesTreeForLayer(MIDDLEGROUND, atlasImageRef);
        ImGui::SameLine();
        displayPropertiesForSelectedSprite();

        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem(s_layersAsString[2]))
    {
        createSpritesTreeForLayer(FOREGROUND, atlasImageRef);
        ImGui::SameLine();
        displayPropertiesForSelectedSprite();

        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Config"))
    {
        ImGui::Checkbox("Debug colliders", &s_debugCollidersEnabled);
        ImGui::Checkbox("Use custom mouse", &s_useCustomMouse);

        ImGui::EndTabItem();
    }

    // Invalidate selected sprite on tab change
    if (lastSelectedTabIndex != s_selectedTabIndex)
    {
        s_selectedSpriteIndex = k_invalidIndex;
    }

    ImGui::EndTabBar();
    ImGui::End();
}