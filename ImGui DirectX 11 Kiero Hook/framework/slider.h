#pragma once

#include "element.h"

#include "animator.h"

class c_slider_int : public c_element
{

private:

    std::string m_format_text{ "%d" };
    int* m_value{};
    int m_min{}, m_max{};

public:

    inline c_slider_int(const std::string_view& name, const std::string_view& label, int* value, int min, int max, const std::string_view& format_str = "%.2f", const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_value = value;
        this->m_min = min;
        this->m_max = max;
        this->m_type = element_type::SLIDER_INT;
        this->m_format_text = format_str;

    }

    int get_min() const { return m_min; }
    void set_min(const int value) { m_min = value; }

    int get_max() const { return m_max; }
    void set_max(const int value) { m_max = value; }

    int get_value() const { return *m_value; }
    void set_value(const int value) { *m_value = value; }

    void set_format_text(const std::string_view& style) { m_format_text = style; }

    virtual bool render()
    {

        ImGui::Text(m_label.c_str());

        const auto ret = ImGui::SliderInt(std::string("##vis_" + m_label).c_str(), m_value, m_min, m_max, m_format_text.c_str());

        return ret;

    }

};

class c_slider_float : public c_element
{

private:

    std::string m_format_text{};
    c_animator<float> m_animator{};
    float* m_value{};
    float m_min{}, m_max{};
    float last_x = 0.f;
    
public:

    inline c_slider_float(const std::string_view& name, const std::string_view& label, float* value, float min, float max, const std::string_view& format_str = "%.2f", const bool default_visibility = true) noexcept
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_value = value;
        this->m_min = min;
        this->m_max = max;
        this->m_type = element_type::SLIDER_FLOAT;
        this->m_format_text = format_str;

    }

    float get_min() const { return m_min; }
    void set_min(const float value) { m_min = value; }

    float get_max() const { return m_max; }
    void set_max(const float value) { m_max = value; }

    float get_value() const { return *m_value; }
    void set_value(const float value) { *m_value = value; }

    void set_format_text(const std::string_view& text) { m_format_text = text; }

    virtual bool render()
    {

        ImGui::Text(m_label.c_str());

        const auto ret = ImGui::SliderFloat(std::string("##vis_" + m_label).c_str(), m_value, m_min, m_max, m_format_text.c_str());

        return ret;

    }

};
