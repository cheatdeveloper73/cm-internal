#pragma once

#include "element.h"

#include "animator.h"

// very similar to a button, except it toggles l0l
class c_tab : public c_element
{

private:

    ImVec2 m_size{};
    c_animator<float> m_animator{ 0.2f, 0.8f, 0.83f };
    bool m_show_name{};
    int* m_current_tab{};
    int m_this_tab{};

public:

    inline c_tab(const std::string_view& name, const std::string_view& label, const ImVec2& size, const int this_tab, int* current_tab, const bool show_name = false, const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_type = element_type::TAB;
        this->m_current_tab = current_tab;
        this->m_this_tab = this_tab;
        this->m_size = size;
        this->m_show_name = show_name;

    }

    virtual bool render()
    {

        static auto custom_button = [](const char* label, ImVec2 esize, bool tabbed, const char* real_name, c_animator<float>* anim = NULL, bool show_name = false) -> bool
        {
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            int flags = 0;
            ImVec2 size_arg = esize;
            if (window->SkipItems)
                return false;

            ImGuiContext& g = *GImGui;
            const ImGuiStyle& style = g.Style;
            const ImGuiID id = window->GetID(label);
            const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

            ImVec2 pos = window->DC.CursorPos;
            if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
                pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
            ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

            const ImRect bb(pos, pos + size);
            ImGui::ItemSize(size, style.FramePadding.y);
            if (!ImGui::ItemAdd(bb, id))
                return false;

            if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
                flags |= ImGuiButtonFlags_Repeat;

            bool hovered, held;
            bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

            // Render
            const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
            ImGui::RenderNavHighlight(bb, id);
            ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

            if (hovered && !tabbed)
                anim->start_animation(false, true);

            anim->update_animation();

            if (tabbed)
                ImGui::TextGradiented(label, bb.GetCenter() - label_size * 0.5f, IM_COL32(0x5c, 0x79, 0x94, 255), IM_COL32(0xbd, 0xe0, 0xfe, 255), 130.f);
            else {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, anim->get_value()));
                const auto tmp_sz = ImGui::CalcTextSize(real_name);
                if (!hovered || !show_name) ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
                else ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, real_name, NULL, &tmp_sz, style.ButtonTextAlign, &bb);
                ImGui::PopStyleColor();
            }

            IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
            return pressed;
        };

        if (*m_current_tab == m_this_tab)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));

        const auto ret = custom_button(m_label.c_str(), m_size, (*m_current_tab == m_this_tab), m_name.c_str(), &m_animator, m_show_name);

        if (*m_current_tab == m_this_tab)
            ImGui::PopStyleColor();

        if (ret) *m_current_tab = m_this_tab;

        return ret;

    }

};
