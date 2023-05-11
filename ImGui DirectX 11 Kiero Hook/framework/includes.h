#pragma once

#include <cstdint>

#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string_view>

#include "config.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#if defined(USE_ICONS)
#include "icons.h" // fix it yoself
#endif

static inline ImVec2  operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
static inline ImVec2  operator/(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
static inline ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2  operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2  operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline ImVec2  operator/(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
static inline ImVec2& operator*=(ImVec2& lhs, const float rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const float rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
static inline ImVec2& operator*=(ImVec2& lhs, const ImVec2& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const ImVec2& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
static inline ImVec4  operator+(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
static inline ImVec4  operator-(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
static inline ImVec4  operator*(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }

// fix for imgui being trash ^

#if defined(DEFINE_CUSTOM_NAMES)
typedef ImVec2 vec2_t;
#endif

// lol ^

// ugh...

namespace ImGui
{

    static void TextGradiented(const char* text, ImU32 leftcolor, ImU32 rightcolor, float smooth = 175) {


        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        const ImVec2 pos = window->DC.CursorPos;
        ImDrawList* draw_list = window->DrawList;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(text);
        const ImVec2 size = CalcTextSize(text);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return;


        const ImU32 col_white = IM_COL32(255, 255, 255, 255);
        float centeredvertex = ImMax((int)smooth, 35);
        float vertex_out = centeredvertex * 0.50f;
        float text_inner = vertex_out - centeredvertex;
        const int vert_start_idx = draw_list->VtxBuffer.Size;
        draw_list->AddText(pos, col_white, text);
        const int vert_end_idx = draw_list->VtxBuffer.Size;
        for (int n = 0; n < 1; n++)
        {
            const ImU32 col_hues[2] = { leftcolor, rightcolor };
            ImVec2 textcenter(pos.x + (size.x / 2), pos.y + (size.y / 2));

            const float a0 = (n) / 6.0f * 2.0f * IM_PI - (0.5f / vertex_out);
            const float a1 = (n + 1.0f) / 6.0f * 2.0f * IM_PI + (0.5f / vertex_out);


            ImVec2 gradient_p0(textcenter.x + ImCos(a0) * text_inner, textcenter.y + ImSin(a0) * text_inner);
            ImVec2 gradient_p1(textcenter.x + ImCos(a1) * text_inner, textcenter.y + ImSin(a1) * text_inner);
            ShadeVertsLinearColorGradientKeepAlpha(draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col_hues[n], col_hues[n + 1]);
        }
    }

    static void TextGradiented(const char* text, ImVec2 pos, ImU32 leftcolor, ImU32 rightcolor, float smooth = 175) {


        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImDrawList* draw_list = window->DrawList;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(text);
        const ImVec2 size = CalcTextSize(text);

        const ImU32 col_white = IM_COL32(255, 255, 255, 255);
        float centeredvertex = ImMax((int)smooth, 35);
        float vertex_out = centeredvertex * 0.50f;
        float text_inner = vertex_out - centeredvertex;
        const int vert_start_idx = draw_list->VtxBuffer.Size;
        draw_list->AddText(pos, col_white, text);
        const int vert_end_idx = draw_list->VtxBuffer.Size;
        for (int n = 0; n < 1; n++)
        {
            const ImU32 col_hues[2] = { leftcolor, rightcolor };
            ImVec2 textcenter(pos.x + (size.x / 2), pos.y + (size.y / 2));

            const float a0 = (n) / 6.0f * 2.0f * IM_PI - (0.5f / vertex_out);
            const float a1 = (n + 1.0f) / 6.0f * 2.0f * IM_PI + (0.5f / vertex_out);


            ImVec2 gradient_p0(textcenter.x + ImCos(a0) * text_inner, textcenter.y + ImSin(a0) * text_inner);
            ImVec2 gradient_p1(textcenter.x + ImCos(a1) * text_inner, textcenter.y + ImSin(a1) * text_inner);
            ShadeVertsLinearColorGradientKeepAlpha(draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col_hues[n], col_hues[n + 1]);
        }
    }

}
