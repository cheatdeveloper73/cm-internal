#pragma once

#include "element.h"

class c_checkbox : public c_element
{

private:

    bool* m_value{};

public:

    inline c_checkbox(const std::string_view& name, const std::string_view& label, bool* value, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_value = value;
        this->m_type = element_type::CHECKBOX;

    }

    bool get_value() const noexcept { return *m_value; }
    void set_value(bool value) noexcept { *m_value = value; }

    virtual bool render()
    {

        static auto custom_checkbox = [](const char* label, bool* v)
        {

            ImGuiWindow* window = ImGui::GetCurrentWindow();
            if (window->SkipItems)
                return false;

            ImGuiContext& g = *GImGui;
            const ImGuiStyle& style = g.Style;
            const ImGuiID id = window->GetID(label);
            const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

            const float square_sz = ImGui::GetFrameHeight();
            const ImVec2 pos = window->DC.CursorPos;
            ImRect total_bb(pos, pos + ImVec2(13.f + label_size.x + 10.f, 16.f));

            ImGui::ItemSize(total_bb, style.FramePadding.y);
            if (!ImGui::ItemAdd(total_bb, id))
                return false;

            bool hovered, held;
            bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
            if (pressed)
            {
                *v = !(*v);
                ImGui::MarkItemEdited(id);
            }

            ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
            check_bb.Min.x -= 5.f;

            auto bg_col = IM_COL32(59, 59, 59, 200);
            auto bdr_col = IM_COL32(114, 114, 114, 200);

            if (hovered && !*v)
                bdr_col = IM_COL32(139, 139, 139, 200);
            else if (hovered && v)
                bdr_col = IM_COL32(139, 139, 139, 100);

            if (!*v) window->DrawList->AddRect(check_bb.Min + ImVec2(6, 3.f), check_bb.Min + ImVec2(6 + 12.f, 12.f + 4.f), bdr_col, 0.f);

            if (*v && !hovered)
                bg_col = IM_COL32(0xa2, 0xd2, 0xff, 170);
            else if (*v && hovered)
                bg_col = IM_COL32(0xa2, 0xd2, 0xff, 230);

            window->DrawList->AddRectFilled(check_bb.Min + ImVec2(7, 1 + 3.f), check_bb.Min + ImVec2(6 + 11.f, 11.f + 4.f), bg_col, 0.f);

            ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x - 5.f, check_bb.Min.y + style.FramePadding.y);
            if (label_size.x > 0.0f)
                ImGui::RenderText(label_pos, label);

            return pressed;

        };

        return custom_checkbox(m_label.c_str(), m_value);

    }

};
