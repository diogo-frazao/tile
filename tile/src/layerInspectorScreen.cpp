#include "screens.h"
#include "imgui.h"
#include "core/app.h"
#include "SDL.h"

int iconSize = 40;
int columns = 5;

void LayerInspectorScreen::update()
{
    ImGui::Begin("Layer Inspector");
    ImGui::BeginTabBar("##Layers");

    if (ImGui::BeginTabItem("Background"))
    {
        ImVec2 spritesTreeSize {ImGui::GetContentRegionAvail().x * 0.7f, 0};


        ImGui::PushItemWidth(100);
        ImGui::SliderInt("Icon Size", &iconSize, 30, 60);
        ImGui::SameLine();
        ImGui::SliderInt("Icons per row", &columns, 2, 8);
        ImGui::PopItemWidth();

        if (ImGui::BeginChild("##SpritesTreeContainer", spritesTreeSize, ImGuiChildFlags_Borders))
        {   
            ImTextureID atlasId = (ImTextureID)(intptr_t)s_atlasTexture;
            D_ASSERT(atlasId != 0, "Invalid texture atlas");
            ImTextureRef ref{atlasId};

            int texW = 0, texH = 0;
            SDL_QueryTexture(s_atlasTexture, nullptr, nullptr, &texW, &texH);

            ImVec2 uv0(static_cast<float>(0) / texW, static_cast<float>(10) / texH);
            ImVec2 uv1(static_cast<float>(0 + 11) / texW, static_cast<float>(10 + 9) / texH);

            if (ImGui::BeginTable("##SpriteTree", columns))
            {
                for (int i = 0; i < 50; ++i)
                {
                    ImGui::TableNextColumn();

                    ImVec2 start_pos = ImGui::GetCursorScreenPos();
                    ImVec2 box_min(start_pos.x, start_pos.y);
                    ImVec2 box_max(box_min.x + iconSize, box_min.y + iconSize);

                    ImDrawList* drawList = ImGui::GetWindowDrawList();
                    drawList->AddRectFilled(box_min, box_max, ImGui::GetColorU32(IM_COL32(35, 35, 35, 220)));
                    drawList->AddImage(ref, box_min, box_max, uv0, uv1);

                    ImGui::Dummy(ImVec2(0.f, iconSize + 6.f));
                }
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();


        ImGui::SameLine();

        if (ImGui::BeginChild("##PropertiesChild"))
        {
            if (ImGui::BeginTable("##Properties", 1))
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Checkbox("Visible", &s_debugCollidersEnabled);
                ImGui::Checkbox("Position", &s_debugCollidersEnabled);
                ImGui::Checkbox("Layer", &s_debugCollidersEnabled);
                ImGui::Checkbox("Is Layer Visible", &s_debugCollidersEnabled);
                
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        ImGui::EndTabItem();
    }


    if (ImGui::BeginTabItem("Middleground"))
    {
        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Foreground"))
    {
        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Config"))
    {
        ImGui::Checkbox("Debug colliders", &s_debugCollidersEnabled);
        ImGui::Checkbox("Use custom mouse", &s_useCustomMouse);
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();

    bool show = true;
    ImGui::ShowDemoWindow(&show);
}