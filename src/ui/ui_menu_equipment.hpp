#pragma once
#include "ui_menu.hpp"
#include "ui_menu_composite_character.hpp"

namespace elona
{
namespace ui
{

class ui_menu_equipment : public ui_menu<ui_menu_composite_character_result>
{
public:
    ui_menu_equipment()
    {
    }

protected:
    virtual bool init();
    virtual void update();
    virtual void draw();
    virtual optional<ui_menu_equipment::result_type> on_key(
        const std::string& key);

private:
    int _cs_prev = 0;
    int _mainhand = 0;

    bool _show_resistances = false;
};

} // namespace ui
} // namespace elona
