#pragma once

#include "includes.h"

template <typename type>
class c_animator
{

private:

    double m_animation_time = 0.001f;
    double m_animation_duration{};
    bool m_hold_anim{};
    bool m_starting_animation{};
    bool m_setup_animations{ false };
    type m_animated_value{};
    type m_start_value{};
    type m_end_value{};

public:

    inline static std::vector<c_animator*> m_animations{};
    inline c_animator() = default;

    inline c_animator(type starting_value, type ending_value, double duration = 1.f) noexcept
    {

        this->m_start_value = starting_value;
        this->m_end_value = ending_value;
        this->m_animation_duration = duration;
        this->m_setup_animations = false;
        this->m_setup_animations = true;
        m_animations.push_back(this);

    }

    bool start_animation(bool starting, bool override = false)
    {

        if (is_animating() && !override)
            return false; // avoid animating when we are already doing it...

        m_animation_time = ImGui::GetTime();
        m_starting_animation = starting;

        return true;

    }

    void update_animation()
    {

        static auto ease = [](double p) {
            if (p < 0.5)
            {
                return 8 * p * p * p * p;
            }
            else
            {
                double f = (p - 1);
                return -8 * f * f * f * f + 1;
            }
        };

        double current_time = ImGui::GetTime() - m_animation_time;

        if (current_time >= m_animation_duration)
        {

            m_animation_time = 0.f;

            if (m_starting_animation)
                m_animated_value = m_end_value;
            else
                m_animated_value = m_start_value;

        }
        else
        {

            double animation_time = ease(current_time / m_animation_duration) * (1.f - ImGui::GetIO().DeltaTime);

            // avoid extrapolating
            animation_time = std::clamp(animation_time, (double)0.f, (double)1.f);

            m_animated_value = std::clamp(ImLerp(m_starting_animation ? m_start_value : m_end_value, m_starting_animation ? m_end_value : m_start_value, animation_time), m_start_value, m_end_value);

        }

    }

    type get_value() const { return m_animated_value; }

    bool get_setup_animator() const { return m_setup_animations; }
    void set_setup_animator(bool val) { m_setup_animations = true; }

    void set_end_value(type val) { m_end_value = val; }
    void set_start_value(type val) { m_start_value = val; }
    void set_duration(double duration) { m_animation_duration = duration; }
    void set_starting_animation(bool val) { m_starting_animation = val; }
    void set_animation_time(type time) { m_animation_time = time; }

    bool is_animating() const { return !(get_animation_percent() == 1 || get_animation_percent() == 0); }

    float get_animation_percent() const { return std::clamp(((((m_animated_value - m_start_value) * 100) / (m_end_value - m_start_value))) / 100.f, 0.f, 100.f); }

};

static void refresh_animations() noexcept
{

    static bool swong = false;

    for (auto animation : c_animator<float>::m_animations) {
        animation->set_animation_time(0.001f);
        animation->start_animation(swong, false);
        swong = !swong;
    }

}
