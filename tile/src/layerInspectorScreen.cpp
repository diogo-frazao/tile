#include "screens.h"
#include "imgui.h"
#include "core/app.h"

void LayerInspectorScreen::update()
{
    ImGui::Begin("Layer Inspector");
    ImGui::BeginTabBar("##Layers");

    if (ImGui::BeginTabItem("Background"))
    {
        ImGui::BeginChild("##SpritesTreeChild", {ImGui::GetContentRegionAvail().x * 0.5f, 0}, true);

        ImGui::BeginTable("##SpritesTree", 1);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Checkbox("Debug colliders", &s_debugCollidersEnabled);
            ImGui::Text("Test");
        ImGui::EndTable();

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("##PropertiesChild");

        ImGui::BeginTable("##Properties", 1);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Checkbox("SomeProperty", &s_debugCollidersEnabled);
            ImGui::Checkbox("SomeOtherProperty", &s_debugCollidersEnabled);
        ImGui::EndTable();

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
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();

    bool show = true;
    ImGui::ShowDemoWindow(&show);
}