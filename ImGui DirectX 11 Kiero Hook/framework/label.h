#pragma once

#include "element.h"

class c_label : public c_element
{

public:

    c_label(const std::string& name, const std::string& label, const bool default_visibility = true)
    {

        this->m_label = label;
        this->m_name = name;
        this->m_visible = default_visibility;
        this->m_type = element_type::LABEL;

    }

    std::string get_text() const { return m_label; }
    void set_text(const std::string& value) { m_label = value; }

    virtual bool render() { ImGui::Text(m_label.c_str()); return false; /* return false cause how a label gon return true... */ }

};
