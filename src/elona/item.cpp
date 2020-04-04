#include "item.hpp"

#include <iostream>
#include <type_traits>

#include "../util/strutil.hpp"
#include "ability.hpp"
#include "activity.hpp"
#include "area.hpp"
#include "audio.hpp"
#include "blending.hpp"
#include "building.hpp"
#include "chara_db.hpp"
#include "character.hpp"
#include "config.hpp"
#include "crafting.hpp"
#include "ctrl_file.hpp"
#include "data/types/type_fish.hpp"
#include "data/types/type_item.hpp"
#include "data/types/type_item_material.hpp"
#include "dmgheal.hpp"
#include "elona.hpp"
#include "enums.hpp"
#include "food.hpp"
#include "fov.hpp"
#include "i18n.hpp"
#include "itemgen.hpp"
#include "lua_env/handle_manager.hpp"
#include "lua_env/lua_env.hpp"
#include "magic.hpp"
#include "map.hpp"
#include "mef.hpp"
#include "message.hpp"
#include "random.hpp"
#include "text.hpp"
#include "ui.hpp"
#include "variables.hpp"

static std::string lang(const std::string& a, const std::string& b)
{
    return jp ? a : b;
}


namespace elona
{


Inventory inv;

int ci_at_m138 = 0;
int p_at_m138 = 0;
int max_at_m138 = 0;
int ti_at_m138 = 0;
elona_vector1<int> list_at_m138;
int f_at_m138 = 0;
int a_at_m138 = 0;


Item::Item()
    : enchantments(15)
{
}



void Item::clear()
{
    copy({}, *this);
}



bool Item::almost_equals(const Item& other, bool ignore_position) const
{
    return true
        // && number == other.number
        && value == other.value && image == other.image &&
        id == other.id
        // && quality == other.quality
        && (ignore_position || position == other.position) &&
        weight == other.weight && identify_state == other.identify_state &&
        count == other.count && dice_x == other.dice_x &&
        dice_y == other.dice_y && damage_bonus == other.damage_bonus &&
        hit_bonus == other.hit_bonus && dv == other.dv && pv == other.pv &&
        skill == other.skill && curse_state == other.curse_state &&
        body_part == other.body_part && function == other.function &&
        enhancement == other.enhancement && own_state == other.own_state &&
        color == other.color && subname == other.subname &&
        material == other.material && param1 == other.param1 &&
        param2 == other.param2 && param3 == other.param3 &&
        param4 == other.param4 &&
        difficulty_of_identification == other.difficulty_of_identification
        // && turn == other.turn
        && _flags == other._flags &&
        range::equal(enchantments, other.enchantments);
}



Inventory::Inventory()
    : storage(ELONA_MAX_ITEMS)
{
    for (size_t i = 0; i < storage.size(); ++i)
    {
        storage[i].index = static_cast<int>(i);
    }
}



InventorySlice Inventory::for_chara(const Character& chara)
{
    if (chara.index == 0)
    {
        return pc();
    }
    else
    {
        return {std::begin(storage) + 200 + 20 * (chara.index - 1),
                std::begin(storage) + 200 + 20 * (chara.index - 1) + 20};
    }
}



InventorySlice Inventory::by_index(int index)
{
    if (index == -1)
    {
        return ground();
    }
    else
    {
        return for_chara(cdata[index]);
    }
}



elona_vector1<int> p_at_m57;
std::string s_;
int skip_ = 0;



/**
 * Tries to find an item in the player's inventory, the ground, or both. Returns
 * the item's reference or none if not found.
 *
 * Valid values of @ref matcher_type:
 *   0: By category
 *   1: By skill
 *   2: By subcategory
 *   3: By ID
 *
 * Valid values of @ref inventory_type:
 *   -1: On ground
 *    0: Both player's inventory and on ground
 *    1: Player's inventory
 */
optional_ref<Item>
item_find(int matcher, int matcher_type, ItemFindLocation location_type)
{
    int result = -1;
    int max_param1 = -1;

    for (int location = 0; location < 2; ++location)
    {
        int invhead;
        int invrange;
        if (location == 0)
        {
            if (location_type == ItemFindLocation::player_inventory)
            {
                continue;
            }
            const auto pair = inv_getheader(-1);
            invhead = pair.first;
            invrange = pair.second;
        }
        else
        {
            if (location_type == ItemFindLocation::ground)
            {
                continue;
            }
            const auto pair = inv_getheader(0);
            invhead = pair.first;
            invrange = pair.second;
        }
        for (int item_index = invhead; item_index < invhead + invrange;
             ++item_index)
        {
            const auto& item = inv[item_index];
            if (item.number() == 0)
            {
                continue;
            }
            if (location == 0)
            {
                if (item.position != cdata.player().position)
                {
                    continue;
                }
            }
            switch (matcher_type)
            {
            case 0:
                if ((int)the_item_db[itemid2int(item.id)]->category == matcher)
                {
                    result = item.index;
                }
                break;
            case 1:
                if (item.skill == matcher)
                {
                    if (max_param1 < item.param1)
                    {
                        result = item.index;
                        max_param1 = item.param1;
                    }
                }
                break;
            case 2:
                if (the_item_db[itemid2int(item.id)]->subcategory == matcher)
                {
                    result = item.index;
                }
                break;
            case 3:
                if (item.id == int2itemid(matcher))
                {
                    result = item.index;
                }
                break;
            default: assert(0); break;
            }
        }
    }

    if (result == -1)
    {
        return none;
    }
    else
    {
        return inv[result];
    }
}



std::vector<std::reference_wrapper<Item>> itemlist(int owner, int id)
{
    std::vector<std::reference_wrapper<Item>> ret;
    for (auto&& item : inv.by_index(owner))
    {
        if (item.number() == 0)
        {
            continue;
        }
        if (item.id == int2itemid(id))
        {
            ret.push_back(item);
        }
    }
    return ret;
}



int itemusingfind(const Item& item, bool disallow_pc)
{
    for (auto&& chara : cdata.all())
    {
        if (chara.state() != Character::State::alive)
        {
            continue;
        }
        if (chara.activity && chara.activity.type != Activity::Type::sex &&
            chara.activity.turn > 0 && chara.activity.item == item.index)
        {
            if (!disallow_pc || chara.index != 0)
            {
                return chara.index;
            }
        }
    }
    return -1; // Not found
}



optional_ref<Item> itemfind(int inventory_id, int matcher, int matcher_type)
{
    if (matcher_type == 0)
    {
        for (auto&& item : inv.for_chara(cdata[inventory_id]))
        {
            if (item.number() == 0)
            {
                continue;
            }
            if (item.id == int2itemid(matcher))
            {
                return item;
            }
        }
        return none;
    }
    else
    {
        for (auto&& item : inv.for_chara(cdata[inventory_id]))
        {
            if (item.number() == 0)
            {
                continue;
            }
            if (the_item_db[itemid2int(item.id)]->subcategory == matcher)
            {
                return item;
            }
        }
        return none;
    }
}



int mapitemfind(int x, int y, int id)
{
    for (const auto& item : inv.ground())
    {
        if (item.number() == 0)
        {
            continue;
        }
        if (item.id == int2itemid(id) && item.position == Position{x, y})
        {
            return item.index;
        }
    }
    return -1; // Not found
}



void cell_refresh(int x, int y)
{
    int p_at_m55 = 0;
    elona_vector1<int> n_at_m55;
    int cnt2_at_m55 = 0;
    int i_at_m55 = 0;
    if (mode == 6 || mode == 9)
    {
        return;
    }
    if (x < 0 || y < 0 || x >= map_data.width || y >= map_data.height)
    {
        return;
    }

    p_at_m55 = 0;
    cell_data.at(x, y).item_appearances_actual = 0;
    cell_data.at(x, y).light = 0;
    for (const auto& item : inv.ground())
    {
        if (item.number() > 0)
        {
            if (item.position.x == x && item.position.y == y)
            {
                floorstack(p_at_m55) = item.index;
                ++p_at_m55;
                wpoke(
                    cell_data.at(x, y).item_appearances_actual, 0, item.image);
                wpoke(
                    cell_data.at(x, y).item_appearances_actual, 2, item.color);
                if (ilight(itemid2int(item.id)) != 0)
                {
                    cell_data.at(x, y).light = ilight(itemid2int(item.id));
                }
            }
        }
    }
    if (p_at_m55 > 3)
    {
        wpoke(
            cell_data.at(x, y).item_appearances_actual,
            0,
            363); // Item bag chip ID
        wpoke(
            cell_data.at(x, y).item_appearances_actual,
            2,
            static_cast<int>(ColorIndex::none));
    }
    else if (p_at_m55 > 1)
    {
        n_at_m55(2) = 0;
        for (int cnt = 0, cnt_end = (p_at_m55); cnt < cnt_end; ++cnt)
        {
            cnt2_at_m55 = cnt;
            i_at_m55 = -1;
            for (int cnt = 0, cnt_end = (p_at_m55); cnt < cnt_end; ++cnt)
            {
                if (cnt2_at_m55 == 1)
                {
                    if (cnt == n_at_m55(0))
                    {
                        continue;
                    }
                }
                if (cnt2_at_m55 == 2)
                {
                    if (cnt == n_at_m55(0) || cnt == n_at_m55(1))
                    {
                        continue;
                    }
                }
                if (inv[floorstack(cnt)].turn > i_at_m55)
                {
                    n_at_m55(cnt2_at_m55) = cnt;
                    i_at_m55 = inv[floorstack(cnt)].turn;
                }
            }
        }
        cell_data.at(x, y).item_appearances_actual =
            floorstack(n_at_m55(0)) - ELONA_ITEM_ON_GROUND_INDEX;
        cell_data.at(x, y).item_appearances_actual +=
            (floorstack(n_at_m55(1)) - ELONA_ITEM_ON_GROUND_INDEX) * 1000;
        if (p_at_m55 > 2)
        {
            cell_data.at(x, y).item_appearances_actual +=
                (floorstack(n_at_m55(2)) - ELONA_ITEM_ON_GROUND_INDEX) *
                1000000;
        }
        else
        {
            cell_data.at(x, y).item_appearances_actual += 999000000;
        }
        cell_data.at(x, y).item_appearances_actual *= -1;
    }
}



void itemturn(Item& item)
{
    ++game_data.item_turns;
    if (game_data.item_turns < 0)
    {
        game_data.item_turns = 1;
    }
    item.turn = game_data.item_turns;
}



void item_copy(int a, int b)
{
    if (a < 0 || b < 0)
        return;

    bool was_empty = inv[b].number() == 0;

    if (was_empty && inv[a].number() > 0)
    {
        // Clean up any stale handles that may have been left over from an item
        // in the same index being removed.
        lua::lua->get_handle_manager().remove_item_handle_run_callbacks(inv[b]);
    }

    Item::copy(inv[a], inv[b]);

    if (was_empty && inv[b].number() != 0)
    {
        lua::lua->get_handle_manager().create_item_handle_run_callbacks(inv[b]);
    }
    else if (!was_empty && inv[b].number() == 0)
    {
        inv[b].remove();
    }
}



void item_exchange(int a, int b)
{
    Item tmp;
    Item::copy(inv[a], tmp);
    Item::copy(inv[b], inv[a]);
    Item::copy(tmp, inv[b]);

    lua::lua->get_handle_manager().swap_handles<Item>(inv[b], inv[a]);
}



void Item::remove()
{
    number_ = 0;
}



void item_delete(Item& item)
{
    const auto index = item.index;
    item.remove();
    item.clear();
    item.index = index; // Restore "index".
}



void item_refresh(Item& i)
{
    if (i.number() <= 0)
    {
        i.remove();
    }
    if (i.index >= ELONA_ITEM_ON_GROUND_INDEX && mode != 6)
    {
        // Refresh the cell the item is on if it's on the ground.
        cell_refresh(i.position.x, i.position.y);
    }
    else if (i.index < 200)
    {
        // Refresh the player's burden state if the item is in their
        // inventory.
        refresh_burden_state();
    }
}



void Item::modify_number(int delta)
{
    this->set_number(this->number_ + delta);
}



void Item::set_number(int number_)
{
    bool item_was_empty = this->number_ <= 0;

    if (item_was_empty && number_ > 0)
    {
        // Clean up any stale handles that may have been left over from an item
        // in the same index being removed.
        lua::lua->get_handle_manager().remove_item_handle_run_callbacks(*this);
    }

    this->number_ = std::max(number_, 0);
    item_refresh(*this);

    bool created_new = item_was_empty && this->number_ > 0;
    if (created_new)
    {
        lua::lua->get_handle_manager().create_item_handle_run_callbacks(*this);
    }
}



int item_separate(int src)
{
    if (inv[src].number() <= 1)
        return src;

    int dst = inv_getfreeid(inv_getowner(src));
    if (dst == -1)
    {
        dst = inv_getfreeid(-1);
        if (dst == -1)
        {
            inv[src].set_number(1);
            txt(i18n::s.get("core.item.something_falls_and_disappears"));
            return src;
        }
    }

    item_copy(src, dst);
    inv[dst].set_number(inv[src].number() - 1);
    inv[src].set_number(1);

    if (inv_getowner(dst) == -1 && mode != 6)
    {
        if (inv_getowner(src) != -1)
        {
            inv[src].position = cdata[inv_getowner(src)].position;
        }
        inv[dst].position = inv[src].position;
        itemturn(inv[dst]);
        cell_refresh(inv[dst].position.x, inv[dst].position.y);
        if (inv_getowner(src) != -1)
        {
            txt(i18n::s.get("core.item.something_falls_from_backpack"));
        }
        refresh_burden_state();
    }

    return dst;
}



bool chara_unequip(Item& item)
{
    if (item.body_part == 0)
        return false;

    int body_part = item.body_part;
    int owner = inv_getowner(item.index);
    if (owner == -1)
        return false;

    cdata[owner].body_parts[body_part - 100] =
        cdata[owner].body_parts[body_part - 100] / 10000 * 10000;
    item.body_part = 0;
    return true;
}



IdentifyState item_identify(Item& item, IdentifyState level)
{
    if (level == IdentifyState::almost &&
        !is_equipment(the_item_db[itemid2int(item.id)]->category))
    {
        level = IdentifyState::completely;
    }
    if (item.identify_state >= level)
    {
        idtresult = IdentifyState::unidentified;
        return idtresult;
    }
    item.identify_state = level;
    if (item.identify_state >= IdentifyState::partly)
    {
        itemmemory(0, itemid2int(item.id)) = 1;
    }
    idtresult = level;
    return idtresult;
}



IdentifyState item_identify(Item& item, int power)
{
    return item_identify(
        item,
        power >= item.difficulty_of_identification
            ? IdentifyState::completely
            : IdentifyState::unidentified);
}



void item_checkknown(Item& item)
{
    if (item.identify_state == IdentifyState::completely) // ???
    {
        item.identify_state = IdentifyState::completely;
    }
    if (itemmemory(0, itemid2int(item.id)) &&
        item.identify_state == IdentifyState::unidentified)
    {
        item_identify(item, IdentifyState::partly);
    }
}



void itemname_additional_info(Item& item)
{
    if (item.id == ItemId::kitty_bank)
    {
        s_ += i18n::s.get_enum("core.item.kitty_bank_rank", item.param2);
    }
    if (item.id == ItemId::bait)
    {
        s_ += lang(
            ""s + i18n::s.get_enum("core.item.bait_rank", item.param1),
            u8" <"s + i18n::s.get_enum("core.item.bait_rank", item.param1) +
                u8">"s);
    }
    if (item.id == ItemId::ancient_book)
    {
        if (jp)
        {
            if (item.param2 != 0)
            {
                s_ += u8"解読済みの"s;
            }
        }
        if (item.identify_state == IdentifyState::completely)
        {
            s_ += lang(
                u8"《"s +
                    i18n::s.get_enum(
                        "core.item.ancient_book_title", item.param1) +
                    u8"》という題名の"s,
                u8" titled <"s +
                    i18n::s.get_enum(
                        "core.item.ancient_book_title", item.param1) +
                    u8">"s);
        }
    }
    if (item.id == ItemId::recipe)
    {
        if (item.param1 == 0)
        {
            s_ += lang(u8"もう使えない"s, u8" which cannot be used anymore"s);
        }
        else if (item.subname == 0)
        {
            s_ += lang(u8"カスタム"s, ""s);
        }
        else
        {
            s_ += lang(
                u8"《"s + rpname(item.subname) + u8"》の"s,
                u8" of <"s + rpname(item.subname) + u8">"s);
        }
    }

    auto category = the_item_db[itemid2int(item.id)]->category;

    if (category == ItemCategory::book)
    {
        if (item.id == ItemId::textbook)
        {
            s_ += lang(
                u8"《"s +
                    i18n::s.get_m(
                        "ability",
                        the_ability_db.get_id_from_legacy(item.param1)->get(),
                        "name") +
                    u8"》という題名の"s,
                u8" titled <Art of "s +
                    i18n::s.get_m(
                        "ability",
                        the_ability_db.get_id_from_legacy(item.param1)->get(),
                        "name") +
                    u8">"s);
        }
        else if (item.id == ItemId::book_of_rachel)
        {
            s_ += lang(u8"第"s, u8" of Rachel No."s) + item.param2 +
                lang(u8"巻目の"s, ""s);
        }
        else if (item.id == ItemId::book_b)
        {
            s_ += lang(
                u8"《"s + booktitle(item.param1) + u8"》という題名の"s,
                u8" titled <"s + booktitle(item.param1) + u8">"s);
        }
    }
    if (category == ItemCategory::altar)
    {
        if (item.param1 != 0)
        {
            s_ += lang(
                god_name(item.param1) + u8"の"s,
                u8" <"s + god_name(item.param1) + u8">"s);
        }
    }
    if (category == ItemCategory::food)
    {
        if (item.param1 != 0)
        {
            if (item.param2 != 0)
            {
                skip_ = 1;
                if (item.id == ItemId::fish_a)
                {
                    s_ = s_ +
                        foodname(
                             item.param1 / 1000,
                             i18n::s.get_m(
                                 "fish",
                                 the_fish_db.get_id_from_legacy(item.subname)
                                     ->get(),
                                 "name"),
                             item.param2,
                             item.subname);
                }
                else
                {
                    s_ = s_ +
                        foodname(
                             item.param1 / 1000,
                             ioriginalnameref(itemid2int(item.id)),
                             item.param2,
                             item.subname);
                }
                return;
            }
        }
        if (item.own_state == 4)
        {
            s_ += lang(""s, u8" grown "s) +
                i18n::s.get_enum("core.ui.weight", item.subname) +
                lang(u8"育った"s, ""s);
        }
    }
    if (item.subname != 0)
    {
        if (item.id == ItemId::fish_a || item.id == ItemId::fish_b)
        {
            if (item.subname < 0 || item.subname >= 100)
            {
                s_ += u8"/bugged/"s;
                return;
            }
            s_ += i18n::s.get_m(
                "fish",
                the_fish_db.get_id_from_legacy(item.subname)->get(),
                "name");
        }
        else if (
            category == ItemCategory::food ||
            category == ItemCategory::bodyparts ||
            item.id == ItemId::figurine || item.id == ItemId::card ||
            item.id == ItemId::shit || item.id == ItemId::bottle_of_milk)
        {
            if (item.subname < 0 || item.subname >= 800)
            {
                s_ += u8"/bugged/"s;
                return;
            }
            if (item.own_state != 4)
            {
                s_ += lang(""s, u8" of "s) +
                    chara_db_get_name(int2charaid(item.subname));
                if (jp)
                {
                    s_ += u8"の"s;
                }
            }
        }
        if (category == ItemCategory::furniture)
        {
            if (jp)
            {
                if (item.subname >= 12)
                {
                    item.subname = 0;
                }
                else
                {
                    s_ += i18n::s.get_enum("core.ui.furniture", item.subname);
                }
            }
        }
        if (item.id == ItemId::deed)
        {
            s_ += lang(""s, u8" of "s) +
                i18n::s.get_enum("core.ui.home", item.param1) +
                lang(u8"の"s, ""s);
        }
        if (item.id == ItemId::bill)
        {
            s_ += lang(
                ""s + item.subname + u8"goldの"s,
                u8" <"s + item.subname + u8" gp>"s);
        }
        if (item.id == ItemId::vomit)
        {
            if (item.subname < 0 || item.subname >= 800)
            {
                s_ += u8"/bugged/"s;
                return;
            }
            s_ += lang(
                ""s + chara_db_get_name(int2charaid(item.subname)) + u8"の"s,
                u8" of "s + chara_db_get_name(int2charaid(item.subname)));
        }
    }
    if (item.id == ItemId::secret_treasure)
    {
        if (item.param1 == 169)
        {
            s_ += lang(u8"善人の"s, u8" of saint"s);
        }
        if (item.param1 == 162)
        {
            s_ += lang(u8"悪人の"s, u8" of wicked"s);
        }
        if (item.param1 == 163)
        {
            s_ += lang(u8"エヘカトルの"s, u8" of Ehekatl"s);
        }
        if (item.param1 == 164)
        {
            s_ += lang(u8"オパートスの"s, u8" of Opatos"s);
        }
        if (item.param1 == 165)
        {
            s_ += lang(u8"イツパロトルの"s, u8" of Itzpalt"s);
        }
        if (item.param1 == 166)
        {
            s_ += lang(u8"ジュアの"s, u8" of Jure"s);
        }
    }
}



std::string itemname(Item& item, int number, bool with_article)
{
    elona_vector1<int> iqiality_;
    int num2_ = 0;
    std::string s2_;
    std::string s3_;
    int alpha_ = 0;
    std::string s4_;
    elona_vector1<std::string> buf_;
    if (itemid2int(item.id) >= maxitemid - 2 ||
        static_cast<size_t>(itemid2int(item.id)) > ioriginalnameref.size())
    {
        return i18n::s.get("core.item.unknown_item");
    }
    if (item.quality >= Quality::godly)
    {
        iqiality_(item.index) = 5;
    }
    item_checkknown(item);
    if (number == 0)
    {
        num2_ = item.number();
    }
    else
    {
        num2_ = number;
    }
    const auto category = the_item_db[itemid2int(item.id)]->category;
    if (jp)
    {
        if (num2_ > 1)
        {
            s2_ = u8"個の"s;
            if (category == ItemCategory::armor)
            {
                s2_ = u8"着の"s;
            }
            if (category == ItemCategory::spellbook ||
                category == ItemCategory::book)
            {
                if (item.id == ItemId::recipe)
                {
                    s2_ = u8"枚の"s;
                }
                else
                {
                    s2_ = u8"冊の"s;
                }
            }
            if (category == ItemCategory::melee_weapon)
            {
                s2_ = u8"本の"s;
            }
            if (category == ItemCategory::potion)
            {
                s2_ = u8"服の"s;
            }
            if (category == ItemCategory::scroll)
            {
                s2_ = u8"巻の"s;
            }
            if (category == ItemCategory::boots ||
                category == ItemCategory::gloves)
            {
                s2_ = u8"対の"s;
            }
            if (category == ItemCategory::gold_piece ||
                category == ItemCategory::platinum_coin ||
                item.id == ItemId::small_medal ||
                item.id == ItemId::music_ticket ||
                item.id == ItemId::token_of_friendship)
            {
                s2_ = u8"枚の"s;
            }
            if (item.id == ItemId::fish_a)
            {
                s2_ = u8"匹の"s;
            }
            s_ = ""s + num2_ + s2_;
        }
        else
        {
            s_ = "";
        }
        if (item.identify_state == IdentifyState::completely)
        {
            switch (item.curse_state)
            {
            case CurseState::doomed:
                s_ += i18n::s.get("core.ui.curse_state.doomed");
                break;
            case CurseState::cursed:
                s_ += i18n::s.get("core.ui.curse_state.cursed");
                break;
            case CurseState::none: break;
            case CurseState::blessed:
                s_ += i18n::s.get("core.ui.curse_state.blessed");
                break;
            }
        }
    }
    else
    {
        s_ = "";
        if (item.identify_state == IdentifyState::completely)
        {
            switch (item.curse_state)
            {
            case CurseState::doomed:
                s_ = i18n::s.get("core.ui.curse_state.doomed") + u8" "s;
                break;
            case CurseState::cursed:
                s_ = i18n::s.get("core.ui.curse_state.cursed") + u8" "s;
                break;
            case CurseState::none: break;
            case CurseState::blessed:
                s_ = i18n::s.get("core.ui.curse_state.blessed") + u8" "s;
                break;
            }
        }
        if (irandomname(itemid2int(item.id)) == 1 &&
            item.identify_state == IdentifyState::unidentified)
        {
            s2_ = "";
        }
        else
        {
            s2_ = ""s + ioriginalnameref2(itemid2int(item.id));
            if (strutil::contains(
                    ioriginalnameref(itemid2int(item.id)), u8"with"))
            {
                s3_ = "";
            }
            else if (strutil::contains(
                         ioriginalnameref(itemid2int(item.id)),
                         u8"for testing"))
            {
                s3_ = "";
            }
            else
            {
                s3_ = u8"of";
            }
            if (item.identify_state != IdentifyState::unidentified && s2_ == "")
            {
                if (item.weight < 0)
                {
                    s2_ = u8"cargo";
                }
                if (category == ItemCategory::boots ||
                    category == ItemCategory::gloves)
                {
                    s2_ = u8"pair";
                }
            }
            if (category == ItemCategory::food && item.param1 != 0 &&
                item.param2 != 0)
            {
                s2_ = u8"dish";
            }
        }
        if (s2_ != ""s)
        {
            if (!s3_.empty())
            {
                s3_ += " ";
            }
            if (num2_ > 1)
            {
                if (s2_ == "variety")
                {
                    s_ = ""s + num2_ + u8" " + s_ + u8"variety " + s3_;
                }
                else if (s2_ == "dish")
                {
                    s_ = ""s + num2_ + u8" " + s_ + u8"dishes " + s3_;
                }
                else
                {
                    s_ = ""s + num2_ + u8" " + s_ + s2_ + u8"s " + s3_;
                }
            }
            else
            {
                s_ = s_ + s2_ + u8" " + s3_;
            }
        }
        else if (num2_ > 1)
        {
            s_ = ""s + num2_ + u8" " + s_;
        }
    }
    if (item.material == 35 && item.param3 < 0)
    {
        if (jp)
        {
            s_ += u8"腐った"s;
        }
        else
        {
            s_ += u8"rotten "s;
        }
    }
    if (en)
    {
        if (category == ItemCategory::food && item.param1 != 0 &&
            item.param2 != 0)
        {
            skip_ = 1;
        }
        if (item.subname != 0 && category == ItemCategory::furniture)
        {
            if (item.subname >= 12)
            {
                item.subname = 0;
            }
            else
            {
                s_ += i18n::s.get_enum("core.ui.furniture", item.subname) +
                    u8" "s;
            }
        }
        if (item.id == ItemId::ancient_book && item.param2 != 0)
        {
            s_ += u8"undecoded "s;
        }
        if (item.id == ItemId::recipe && item.subname == 0)
        {
            s_ += u8"custom "s;
        }
    }
    if (item.id == ItemId::material_kit)
    {
        s_ += ""s +
            i18n::s.get_m(
                "item_material",
                the_item_material_db.get_id_from_legacy(item.material)->get(),
                "name") +
            lang(u8"製の"s, u8" "s);
    }
    if (jp)
    {
        itemname_additional_info(item);
    }
    if (category == ItemCategory::furniture && item.material != 0)
    {
        if (jp)
        {
            s_ += ""s +
                i18n::s.get_m(
                    "item_material",
                    the_item_material_db.get_id_from_legacy(item.material)
                        ->get(),
                    "name") +
                u8"細工の"s;
        }
        else
        {
            s_ += ""s +
                i18n::s.get_m(
                    "item_material",
                    the_item_material_db.get_id_from_legacy(item.material)
                        ->get(),
                    "name") +
                u8"work "s;
        }
    }
    if (item.id == ItemId::gift)
    {
        s_ += i18n::s.get_enum("core.item.gift_rank", item.param4) +
            i18n::space_if_needed();
    }
    if (skip_ != 1)
    {
        alpha_ = 0;
        if (item.identify_state == IdentifyState::completely &&
            is_equipment(category))
        {
            if (item.is_eternal_force())
            {
                alpha_ = 1;
                s_ += lang(u8"エターナルフォース"s, u8"eternal force"s) +
                    i18n::space_if_needed();
            }
            else
            {
                if (item.subname >= 10000)
                {
                    if (item.subname < 20000)
                    {
                        if (jp)
                        {
                            s_ += egoname(item.subname - 10000) +
                                i18n::space_if_needed();
                        }
                    }
                    else if (item.subname < 40000)
                    {
                        s_ += egominorn(item.subname - 20000) +
                            i18n::space_if_needed();
                    }
                }
                if (item.quality != Quality::special)
                {
                    if (item.quality >= Quality::miracle)
                    {
                        s_ += i18n::s.get_m(
                                  "item_material",
                                  the_item_material_db
                                      .get_id_from_legacy(item.material)
                                      ->get(),
                                  "alias") +
                            i18n::space_if_needed();
                    }
                    else
                    {
                        s_ += i18n::s.get_m(
                                  "item_material",
                                  the_item_material_db
                                      .get_id_from_legacy(item.material)
                                      ->get(),
                                  "name") +
                            i18n::space_if_needed();
                        if (jp)
                        {
                            if (/* TODO is_katakana */ false)
                            {
                                alpha_ = 1;
                            }
                            else
                            {
                                s_ += u8"の"s;
                            }
                        }
                    }
                }
            }
        }
        if (item.identify_state == IdentifyState::unidentified)
        {
            s_ += iknownnameref(itemid2int(item.id));
        }
        else if (item.identify_state != IdentifyState::completely)
        {
            if (item.quality < Quality::miracle || !is_equipment(category))
            {
                s_ += ioriginalnameref(itemid2int(item.id));
            }
            else
            {
                s_ += iknownnameref(itemid2int(item.id));
            }
        }
        else if (item.quality == Quality::special || item.is_precious())
        {
            if (jp)
            {
                s_ = u8"★"s + s_ + ioriginalnameref(itemid2int(item.id));
            }
            else
            {
                s_ += ioriginalnameref(itemid2int(item.id));
            }
        }
        else
        {
            if (item.quality >= Quality::miracle && jp)
            {
                s_ = u8"☆"s + s_;
            }
            if (alpha_ == 1 && jp)
            {
                s_ += ialphanameref(itemid2int(item.id));
            }
            else
            {
                s_ += ioriginalnameref(itemid2int(item.id));
            }
            if (en && is_equipment(category) && item.subname >= 10000 &&
                item.subname < 20000)
            {
                s_ += u8" "s + egoname((item.subname - 10000));
            }
            if (item.subname >= 40000)
            {
                randomize(item.subname - 40000);
                if (item.quality == Quality::miracle)
                {
                    s_ += i18n::space_if_needed() +
                        i18n::s.get(
                            "core.item.miracle_paren",
                            random_title(RandomTitleType::weapon));
                }
                else
                {
                    s_ += i18n::space_if_needed() +
                        i18n::s.get(
                            "core.item.godly_paren",
                            random_title(RandomTitleType::weapon));
                }
                randomize();
            }
        }
    }
    if (en)
    {
        if (with_article)
        {
            if (item.identify_state == IdentifyState::completely &&
                (item.quality >= Quality::miracle && is_equipment(category)))
            {
                s_ = u8"the "s + s_;
            }
            else if (num2_ == 1)
            {
                s4_ = strmid(s_, 0, 1);
                if (s4_ == u8"a"s || s4_ == u8"o"s || s4_ == u8"i"s ||
                    s4_ == u8"u"s || s4_ == u8"e"s)
                {
                    s_ = u8"an "s + s_;
                }
                else
                {
                    s_ = u8"a "s + s_;
                }
            }
        }
        if (s2_ == "" && item.id != ItemId::fish_a && num2_ > 1)
        {
            switch (s_.back())
            {
            case 'f':
                if (strutil::ends_with(s_, "staff"))
                {
                    s_ = s_.substr(0, s_.size() - 2) + "ves"; // staff -> staves
                }
                else
                {
                    s_ =
                        s_.substr(0, s_.size() - 1) + "ves"; // shelf -> shelves
                }
                break;
            case 's':
                if (!strutil::ends_with(s_, "stairs"))
                {
                    s_ += "es"; // upstairs -> upstairs / dress -> dresses
                }
                break;
            case 'y':
                if (strutil::ends_with(s_, "key") ||
                    strutil::ends_with(s_, "toy"))
                {
                    s_ += "s"; // key -> keys / toy -> toys
                }
                else
                {
                    s_ = s_.substr(0, s_.size() - 1) +
                        "ies"; // cherry -> cherries
                }
                break;
            case 'o':
                if (strutil::ends_with(s_, "tomato") ||
                    strutil::ends_with(s_, "potato"))
                {
                    s_ += "es"; // tomato -> tomatoes / potato -> potatoes
                }
                else
                {
                    s_ += "s"; // piano -> pianos
                }
                break;
            case 'x':
                s_ += "es"; // box -> boxes
                break;
            case 'h':
                if (strutil::ends_with(s_, "sh"))
                {
                    s_ += "es"; // dish -> dishes
                }
                else if (strutil::ends_with(s_, "ch"))
                {
                    s_ += "es"; // torch -> torches
                }
                else
                {
                    s_ += "s";
                }
                break;
            default: s_ += "s"; break;
            }
        }
        itemname_additional_info(item);
    }
    if (item.identify_state == IdentifyState::completely)
    {
        if (item.enhancement != 0)
        {
            s_ += ""s + cnvfix(item.enhancement) + u8" "s;
        }
        if (item.has_charge())
        {
            s_ += i18n::s.get("core.item.charges", item.count);
        }
        if (item.dice_x != 0 || item.hit_bonus != 0 || item.damage_bonus != 0)
        {
            s_ += u8" ("s;
            if (item.dice_x != 0)
            {
                s_ += ""s + item.dice_x + u8"d"s + item.dice_y;
                if (item.damage_bonus != 0)
                {
                    if (item.damage_bonus > 0)
                    {
                        s_ += u8"+"s + item.damage_bonus;
                    }
                    else
                    {
                        s_ += ""s + item.damage_bonus;
                    }
                }
                s_ += u8")"s;
                if (item.hit_bonus != 0)
                {
                    s_ += u8"("s + item.hit_bonus + u8")"s;
                }
            }
            else
            {
                s_ +=
                    ""s + item.hit_bonus + u8","s + item.damage_bonus + u8")"s;
            }
        }
        if (item.dv != 0 || item.pv != 0)
        {
            s_ += u8" ["s + item.dv + u8","s + item.pv + u8"]"s;
        }
    }
    if (en && (item.id == ItemId::wallet || item.id == ItemId::suitcase))
    {
        s_ += u8"(Lost property)"s;
    }
    if (item.id == ItemId::fishing_pole && item.count != 0)
    {
        s_ += lang(
            u8"("s + i18n::s.get_enum("core.item.bait_rank", item.param4) +
                u8"残り"s + item.count + u8"匹)"s,
            u8"("s + item.count + u8" "s +
                i18n::s.get_enum("core.item.bait_rank", item.param4) + u8")"s);
    }
    if (item.id == ItemId::monster_ball)
    {
        if (item.subname == 0)
        {
            s_ += lang(u8" Lv"s, u8" Level "s) + item.param2 +
                lang(u8" (空)"s, u8"(Empty)"s);
        }
        else
        {
            s_ +=
                u8" ("s + chara_db_get_name(int2charaid(item.subname)) + u8")"s;
        }
    }
    if (item.id == ItemId::small_gamble_chest)
    {
        s_ += lang(u8" Lv"s, u8" Level "s) + item.param2;
    }
    if (item.identify_state == IdentifyState::almost && is_equipment(category))
    {
        s_ += u8" ("s +
            cnven(i18n::s.get_enum(
                u8"core.ui.quality", static_cast<int>(item.quality))) +
            u8")"s;
        if (jp)
        {
            s_ += u8"["s +
                i18n::s.get_m(
                    "item_material",
                    the_item_material_db.get_id_from_legacy(item.material)
                        ->get(),
                    "name") +
                u8"製]"s;
        }
        else
        {
            s_ += u8"["s +
                cnven(i18n::s.get_m(
                    "item_material",
                    the_item_material_db.get_id_from_legacy(item.material)
                        ->get(),
                    "name")) +
                u8"]"s;
        }
        if (item.curse_state == CurseState::cursed)
        {
            s_ += i18n::s.get("core.item.approximate_curse_state.cursed");
        }
        if (item.curse_state == CurseState::doomed)
        {
            s_ += i18n::s.get("core.item.approximate_curse_state.doomed");
        }
    }
    if (category == ItemCategory::chest)
    {
        if (item.id == ItemId::shopkeepers_trunk)
        {
            s_ += lang(u8"(移動時消滅)"s, u8"(Temporal)"s);
        }
        else if (item.count == 0)
        {
            if (item.param1 == 0)
            {
                s_ += lang(u8"(空っぽ)"s, u8"(Empty)"s);
            }
        }
    }
    if (category == ItemCategory::cargo && item.param2 != 0)
    {
        s_ += lang(
            u8"(仕入れ値 "s + item.param2 + u8"g)"s,
            u8"(Buying price: "s + item.param2 + u8")"s);
    }
    if (item.is_aphrodisiac())
    {
        s_ += lang(u8"(媚薬混入)"s, u8"(Aphrodisiac)"s);
    }
    if (item.is_poisoned())
    {
        s_ += lang(u8"(毒物混入)"s, u8"(Poisoned)"s);
    }
    if (item.has_cooldown_time() && game_data.date.hours() < item.count)
    {
        s_ += lang(
            u8"("s + (item.count - game_data.date.hours()) + u8"時間)"s,
            u8"(Next: "s + (item.count - game_data.date.hours()) + u8"h.)"s);
    }
    if (item.id == ItemId::shelter && item.count != 0)
    {
        s_ += lang(u8" シリアルNo."s, u8" serial no."s) + item.count;
    }
    if (item.id == ItemId::disc)
    {
        s_ += u8" <BGM"s + item.param1 + u8">"s;
    }
    if (strlen_u(s_) > 66)
    {
        s_ = zentohan(s_);
    }
    skip_ = 0;
    return s_;
}



void remain_make(Item& remain, const Character& chara)
{
    remain.subname = charaid2int(chara.id);
    remain.color = chara.image / 1000;

    if (remain.id == ItemId::corpse)
    {
        remain.weight = 250 * (chara.weight + 100) / 100 + 500;
        remain.value = remain.weight / 5;
    }
    else
    {
        remain.weight = 20 * (chara.weight + 500) / 500;
        remain.value = chara.level * 40 + 600;
        if (the_character_db[charaid2int(chara.id)]->rarity / 1000 < 20 &&
            chara.original_relationship < -1)
        {
            remain.value *= clamp(
                4 - the_character_db[charaid2int(chara.id)]->rarity / 1000 / 5,
                1,
                5);
        }
    }
}



bool item_stack(int inventory_id, Item& base_item, bool show_message)
{
    if (base_item.quality == Quality::special &&
        is_equipment(the_item_db[itemid2int(base_item.id)]->category))
    {
        return 0;
    }

    bool did_stack = false;

    for (auto&& item : inv.by_index(inventory_id))
    {
        if (item.index == base_item.index || item.number() == 0 ||
            item.id != base_item.id)
            continue;

        bool stackable;
        if (item.id == ItemId::small_medal)
            stackable = inventory_id != -1 || mode == 6 ||
                item.position == base_item.position;
        else
            stackable =
                item.almost_equals(base_item, inventory_id != -1 || mode == 6);

        if (stackable)
        {
            item.modify_number(base_item.number());
            base_item.remove();
            ti = item.index;
            did_stack = true;
            break;
        }
    }

    if (did_stack)
    {
        if (mode != 6 && inv_getowner(base_item.index) == -1)
        {
            cell_refresh(base_item.position.x, base_item.position.y);
        }
        if (show_message)
        {
            txt(i18n::s.get("core.item.stacked", inv[ti], inv[ti].number()));
        }
    }

    return did_stack;
}



void item_dump_desc(const Item& i)
{
    reftype = (int)the_item_db[itemid2int(i.id)]->category;

    item_db_get_charge_level(inv[ci], itemid2int(i.id));
    item_db_get_description(inv[ci], itemid2int(i.id));

    p = item_load_desc(inv[ci]);

    listmax = p;
    pagemax = (listmax - 1) / pagesize;
}



void item_acid(const Character& owner, int ci)
{
    if (ci == -1)
    {
        for (const auto& body_part : owner.body_parts)
        {
            if (body_part / 10000 == 0)
            {
                break;
            }
            int i = body_part % 10000 - 1;
            if (i == -1)
            {
                continue;
            }
            if (inv[i].enhancement >= -3)
            {
                if (rnd(30) == 0)
                {
                    ci = p;
                    break;
                }
            }
        }
        if (ci == -1)
        {
            return;
        }
    }

    if (!is_equipment(the_item_db[itemid2int(inv[ci].id)]->category))
    {
        return;
    }

    if (inv[ci].is_acidproof())
    {
        txt(i18n::s.get("core.item.acid.immune", owner, inv[ci]));
    }
    else
    {
        txt(i18n::s.get("core.item.acid.damaged", owner, inv[ci]),
            Message::color{ColorIndex::purple});
        --inv[ci].enhancement;
    }
}



bool item_fire(int owner, optional_ref<Item> burned_item)
{
    optional_ref<Item> blanket;
    std::vector<std::reference_wrapper<Item>> list;
    if (burned_item)
    {
        list.push_back(*burned_item);
    }

    if (owner != -1)
    {
        if (sdata(50, owner) / 50 >= 6 ||
            cdata[owner].quality >= Quality::miracle)
        {
            return false;
        }
        for (auto&& item : inv.by_index(owner))
        {
            if (item.number() == 0)
            {
                continue;
            }
            if (item.id == ItemId::fireproof_blanket)
            {
                if (!blanket)
                {
                    blanket = item;
                }
                continue;
            }
            if (!burned_item)
            {
                list.push_back(item);
            }
        }
    }

    if (list.empty())
    {
        return false;
    }

    bool burned{};

    for (int _i = 0; _i < 3; ++_i)
    {
        auto& item = choice(list).get();
        if (item.number() <= 0)
        {
            continue;
        }
        rowact_item(item);
        if (item.is_fireproof() || item.is_precious())
        {
            continue;
        }

        const auto category = the_item_db[itemid2int(item.id)]->category;
        if (category == ItemCategory::food && item.param2 == 0)
        {
            if (owner == -1)
            {
                if (is_in_fov(item.position))
                {
                    txt(i18n::s.get(
                            "core.item.item_on_the_ground_get_broiled", item),
                        Message::color{ColorIndex::gold});
                }
            }
            else
            {
                if (is_in_fov(cdata[owner]))
                {
                    txt(i18n::s.get(
                            "core.item.item_on_the_ground_get_broiled",
                            item,
                            cdata[owner]),
                        Message::color{ColorIndex::gold});
                }
            }
            make_dish(item, rnd(5) + 1);
            burned = true;
            continue;
        }

        if (category == ItemCategory::chest || category == ItemCategory::tool ||
            category == ItemCategory::gold_piece ||
            item.quality >= Quality::miracle)
        {
            continue;
        }

        if (item.body_part != 0)
        {
            if (rnd(2))
            {
                continue;
            }
        }

        if (category != ItemCategory::rod && category != ItemCategory::tree &&
            category != ItemCategory::book &&
            category != ItemCategory::scroll &&
            category != ItemCategory::spellbook)
        {
            if (rnd(4))
            {
                continue;
            }
            if (owner != -1)
            {
                if (rnd(4))
                {
                    continue;
                }
            }
        }

        if (blanket)
        {
            item_separate(blanket->index);
            if (is_in_fov(cdata[owner]))
            {
                txt(i18n::s.get(
                    "core.item.fireproof_blanket_protects_item",
                    *blanket,
                    cdata[owner]));
            }
            if (blanket->count > 0)
            {
                --blanket->count;
            }
            else if (rnd(20) == 0)
            {
                blanket->modify_number(-1);
                if (is_in_fov(cdata[owner]))
                {
                    txt(i18n::s.get(
                        "core.item.fireproof_blanket_turns_to_dust", *blanket));
                }
                break;
            }
            continue;
        }

        int p_ = rnd_capped(item.number()) / 2 + 1;
        if (owner != -1)
        {
            if (item.body_part != 0)
            {
                if (is_in_fov(cdata[owner]))
                {
                    txt(i18n::s.get(
                            "core.item.item_someone_equips_turns_to_dust",
                            itemname(item, p_),
                            p_,
                            cdata[owner]),
                        Message::color{ColorIndex::purple});
                }
                cdata[owner].body_parts[item.body_part - 100] =
                    cdata[owner].body_parts[item.body_part - 100] / 10000 *
                    10000;
                item.body_part = 0;
                chara_refresh(owner);
            }
            else if (is_in_fov(cdata[owner]))
            {
                txt(i18n::s.get(
                        "core.item.someones_item_turns_to_dust",
                        itemname(item, p_, false),
                        p_,
                        cdata[owner]),
                    Message::color{ColorIndex::purple});
            }
        }
        else if (is_in_fov(item.position))
        {
            txt(i18n::s.get(
                    "core.item.item_on_the_ground_turns_to_dust",
                    itemname(item, p_),
                    p_),
                Message::color{ColorIndex::purple});
        }
        item.modify_number(-p_);
        cell_refresh(item.position.x, item.position.y);
        burned = true;
    }

    refresh_burden_state();
    return burned;
}



void mapitem_fire(int x, int y)
{
    if (cell_data.at(x, y).item_appearances_actual == 0)
    {
        return;
    }

    optional_ref<Item> burned_item;
    for (auto&& item : inv.ground())
    {
        if (item.number() == 0)
        {
            continue;
        }
        if (item.position == Position{x, y})
        {
            burned_item = item;
            break;
        }
    }
    if (burned_item)
    {
        const auto burned = item_fire(-1, burned_item);
        if (burned)
        {
            if (cell_data.at(x, y).mef_index_plus_one == 0)
            {
                mef_add(x, y, 5, 24, rnd(10) + 5, 100, cc);
            }
        }
        cell_refresh(x, y);
    }
}



bool item_cold(int owner, optional_ref<Item> destroyed_item)
{
    optional_ref<Item> blanket;
    std::vector<std::reference_wrapper<Item>> list;
    if (destroyed_item)
    {
        list.push_back(*destroyed_item);
    }
    if (owner != -1)
    {
        if (sdata(51, owner) / 50 >= 6 ||
            cdata[owner].quality >= Quality::miracle)
        {
            return false;
        }
        for (auto&& item : inv.by_index(owner))
        {
            if (item.number() == 0)
            {
                continue;
            }
            if (item.id == ItemId::coldproof_blanket)
            {
                if (!blanket)
                {
                    blanket = item;
                }
                continue;
            }
            if (!destroyed_item)
            {
                list.push_back(item);
            }
        }
    }
    if (list.empty())
    {
        return false;
    }

    bool broken{};
    for (int _i = 0; _i < 2; ++_i)
    {
        auto& item = choice(list).get();
        if (item.number() <= 0)
        {
            continue;
        }
        rowact_item(item);
        if (item.is_precious())
        {
            continue;
        }

        const auto category = the_item_db[itemid2int(item.id)]->category;
        if (category == ItemCategory::chest || category == ItemCategory::tool ||
            category == ItemCategory::gold_piece)
        {
            continue;
        }
        if (item.quality >= Quality::miracle || item.body_part != 0)
        {
            continue;
        }
        if (category != ItemCategory::potion)
        {
            if (rnd(30))
            {
                continue;
            }
        }
        if (blanket)
        {
            item_separate(blanket->index);
            if (is_in_fov(cdata[owner]))
            {
                txt(i18n::s.get(
                    "core.item.coldproof_blanket_protects_item",
                    *blanket,
                    cdata[owner]));
            }
            if (blanket->count > 0)
            {
                --blanket->count;
            }
            else if (rnd(20) == 0)
            {
                blanket->modify_number(-1);
                if (is_in_fov(cdata[owner]))
                {
                    txt(i18n::s.get(
                        "core.item.coldproof_blanket_is_broken_to_pieces",
                        *blanket));
                }
                break;
            }
            continue;
        }
        int p_ = rnd_capped(item.number()) / 2 + 1;
        if (owner != -1)
        {
            if (is_in_fov(cdata[owner]))
            {
                txt(i18n::s.get(
                        "core.item.someones_item_breaks_to_pieces",
                        itemname(item, p_, false),
                        p_,
                        cdata[owner]),
                    Message::color{ColorIndex::purple});
            }
        }
        else if (is_in_fov(item.position))
        {
            txt(i18n::s.get(
                    "core.item.item_on_the_ground_breaks_to_pieces",
                    itemname(item, p_),
                    p_),
                Message::color{ColorIndex::purple});
        }
        item.modify_number(-p_);
        broken = true;
    }

    refresh_burden_state();
    return broken;
}



void mapitem_cold(int x, int y)
{
    if (cell_data.at(x, y).item_appearances_actual == 0)
    {
        return;
    }
    optional_ref<Item> destroyed_item;
    for (auto&& item : inv.ground())
    {
        if (item.number() == 0)
        {
            continue;
        }
        if (item.position == Position{x, y})
        {
            destroyed_item = item;
            break;
        }
    }
    if (destroyed_item)
    {
        item_cold(-1, destroyed_item);
        cell_refresh(x, y);
    }
}



Item& get_random_inv(int owner)
{
    const auto tmp = inv_getheader(owner);
    const auto index = tmp.first + rnd(tmp.second);
    return inv[index];
}



std::pair<int, int> inv_getheader(int owner)
{
    if (owner == 0)
    {
        return {0, 200};
    }
    else if (owner == -1)
    {
        return {ELONA_ITEM_ON_GROUND_INDEX, 400};
    }
    else
    {
        return {200 + 20 * (owner - 1), 20};
    }
}



int inv_getowner(int inv_id)
{
    if (inv_id < 200)
    {
        return 0;
    }
    if (inv_id >= ELONA_ITEM_ON_GROUND_INDEX)
    {
        return -1;
    }
    return (inv_id - 200) / 20 + 1;
}



int inv_find(int id, int owner)
{
    for (const auto& item : inv.for_chara(cdata[owner]))
    {
        if (item.number() == 0)
        {
            continue;
        }
        if (item.id == int2itemid(id))
        {
            return item.index;
        }
    }
    return -1; // Not found
}

bool inv_getspace(int owner)
{
    for (const auto& item : inv.by_index(owner))
    {
        if (item.number() == 0)
        {
            return true;
        }
    }
    return false;
}

int inv_sum(int owner)
{
    int n{};
    for (const auto& item : inv.by_index(owner))
    {
        if (item.number() != 0)
        {
            ++n;
        }
    }
    return n;
}

int inv_compress(int owner)
{
    int number_of_deleted_items{};
    for (int i = 0; i < 100; ++i)
    {
        int threshold = 200 * (i * i + 1);
        for (auto&& item : inv.by_index(owner))
        {
            if (item.number() != 0)
            {
                if (!item.is_precious() && item.value < threshold)
                {
                    item.remove();
                    ++number_of_deleted_items;
                    if (item.position.x >= 0 &&
                        item.position.x < map_data.width &&
                        item.position.y >= 0 &&
                        item.position.y < map_data.height)
                    {
                        cell_refresh(item.position.x, item.position.y);
                    }
                }
            }
            if (number_of_deleted_items > 10)
            {
                break;
            }
        }
        if (number_of_deleted_items > 10)
        {
            break;
        }
    }

    int slot = -1;
    for (const auto& item : inv.by_index(owner))
    {
        if (item.number() == 0)
        {
            slot = item.index;
            break;
        }
    }

    if (slot == -1)
    {
        // Destroy 1 existing item forcely.
        auto&& item = get_random_inv(owner);
        slot = item.index;
        item.remove();
        if (mode != 6)
        {
            if (item.position.x >= 0 && item.position.x < map_data.width &&
                item.position.y >= 0 && item.position.y < map_data.height)
            {
                cell_refresh(item.position.x, item.position.y);
            }
        }
    }

    return slot;
}

int inv_getfreeid(int owner)
{
    for (const auto& item : inv.by_index(owner))
    {
        if (item.number() == 0)
        {
            return item.index;
        }
    }
    if (owner == -1 && mode != 6)
    {
        txt(i18n::s.get("core.item.items_are_destroyed"));
        return inv_compress(owner);
    }
    return -1;
}

int inv_weight(int owner)
{
    int weight{};
    if (owner == 0)
    {
        game_data.cargo_weight = 0;
    }
    for (const auto& item : inv.by_index(owner))
    {
        if (item.number() != 0)
        {
            if (item.weight >= 0)
            {
                weight += item.weight * item.number();
            }
            else if (owner == 0)
            {
                game_data.cargo_weight += -item.weight * item.number();
            }
        }
    }
    return weight;
}

int inv_getfreeid_force()
{
    p = inv_getfreeid(tc);
    if (p != -1)
    {
        return p;
    }
    for (int cnt = 0; cnt < 100; ++cnt)
    {
        p = rnd(invrange) + invhead;
        if (inv[p].body_part == 0)
        {
            inv[p].remove();
            if (cdata[tc].item_which_will_be_used == p)
            {
                cdata[tc].item_which_will_be_used = 0;
            }
            break;
        }
    }
    return p;
}



void item_drop(Item& item_in_inventory, int num, bool building_shelter)
{
    ti = inv_getfreeid(-1);
    if (ti == -1)
    {
        txt(i18n::s.get("core.action.drop.too_many_items"));
        update_screen();
        return;
    }

    item_copy(item_in_inventory.index, ti);
    inv[ti].position = cdata[cc].position;
    inv[ti].set_number(num);
    itemturn(inv[ti]);

    if (building_shelter)
    {
        inv[ti].own_state = 3;
        inv[ti].count = game_data.next_shelter_serial_id + 100;
        ++game_data.next_shelter_serial_id;
    }
    else
    {
        snd("core.drop1");
        txt(i18n::s.get("core.action.drop.execute", itemname(inv[ti], num)));
    }

    if (inv[ti].id == ItemId::bottle_of_water) // Water
    {
        if (const auto altar = item_find(60002, 0))
        {
            // The altar is your god's.
            if (core_god::int2godid(altar->param1) == cdata[cc].god_id)
            {
                if (inv[ti].curse_state != CurseState::blessed)
                {
                    snd("core.pray1");
                    inv[ti].curse_state = CurseState::blessed;
                    txt(i18n::s.get("core.action.drop.water_is_blessed"),
                        Message::color{ColorIndex::green});
                }
            }
        }
    }

    item_stack(-1, inv[ti]);
    item_in_inventory.modify_number(-num);

    refresh_burden_state();
    cell_refresh(inv[ti].position.x, inv[ti].position.y);
    screenupdate = -1;
    update_screen();

    if (area_data[game_data.current_map].id == mdata_t::MapId::museum)
    {
        if (mode == 0)
        {
            update_museum();
        }
    }
    if (game_data.current_map == mdata_t::MapId::your_home)
    {
        if (mode == 0)
        {
            calc_home_rank();
        }
    }
    if (inv[ti].id == ItemId::campfire)
    {
        map_data.play_campfire_sound = 1;
        play_music();
    }
}



void item_build_shelter(Item& shelter)
{
    item_drop(shelter, 1, true);
}



int iequiploc(const Item& item)
{
    switch (the_item_db[itemid2int(item.id)]->category)
    {
    case ItemCategory::helm: return 1;
    case ItemCategory::necklace: return 2;
    case ItemCategory::cloak: return 3;
    case ItemCategory::armor: return 4;
    case ItemCategory::melee_weapon: return 5;
    case ItemCategory::shield: return 5;
    case ItemCategory::ring: return 6;
    case ItemCategory::gloves: return 7;
    case ItemCategory::boots: return 9;
    case ItemCategory::ranged_weapon: return 10;
    case ItemCategory::ammo: return 11;
    case ItemCategory::belt: return 8;
    default: return 0;
    }
}



std::vector<int> item_get_inheritance(const Item& item)
{
    std::vector<int> result;

    randomize(item.turn + 1);
    for (int _i = 0; _i < 10; ++_i)
    {
        const auto enc_index = rnd(15);
        if (enc_index == 0)
            continue;
        if (item.enchantments[enc_index].id == 0)
            continue;

        const auto exists = range::find(result, enc_index) != std::end(result);
        if (exists)
            continue;

        if (item.enchantments[enc_index].power < 0 ||
            static_cast<int>(result.size()) < rnd(4))
        {
            result.push_back(enc_index);
        }
    }
    randomize();

    return result;
}



void auto_identify()
{
    if (cdata.player().confused != 0 || cdata.player().sleep != 0 ||
        cdata.player().paralyzed != 0 || cdata.player().choked != 0)
    {
        return;
    }

    for (auto&& item : inv.pc())
    {
        if (item.number() == 0 ||
            item.identify_state == IdentifyState::completely)
        {
            continue;
        }
        if (!is_equipment(the_item_db[itemid2int(item.id)]->category))
        {
            continue;
        }

        const auto skill = sdata(13, 0) + sdata(162, 0) * 5;
        const auto difficulty = 1500 + item.difficulty_of_identification * 5;
        if (skill > rnd(difficulty * 5))
        {
            const auto prev_name = itemname(item);
            item_identify(item, IdentifyState::completely);
            itemmemory(0, itemid2int(item.id)) = 1;
            if (!g_config.hide_autoidentify())
            {
                txt(i18n::s.get(
                    "core.misc.identify.fully_identified", prev_name, item));
            }
            chara_gain_skill_exp(cdata.player(), 162, 50);
        }
        if (item.identify_state <= IdentifyState::partly)
        {
            if (skill > rnd(difficulty))
            {
                if (!g_config.hide_autoidentify())
                {
                    txt(i18n::s.get(
                        "core.misc.identify.almost_identified",
                        item,
                        i18n::s.get_enum(
                            u8"core.ui.quality",
                            static_cast<int>(item.quality))));
                }
                item_identify(item, IdentifyState::almost);
                chara_gain_skill_exp(cdata.player(), 162, 50);
            }
        }
    }
}



void begintempinv()
{
    ctrl_file(FileOperation2::map_items_write, u8"shoptmp.s2");
    for (auto&& item : inv.ground())
    {
        item.remove();
    }
}



void exittempinv()
{
    ctrl_file(FileOperation2::map_items_read, u8"shoptmp.s2");
}



bool cargocheck(const Item& item)
{
    if (!the_item_db[itemid2int(item.id)]->is_cargo)
        return true;

    if (map_data.type != mdata_t::MapType::world_map &&
        map_data.type != mdata_t::MapType::player_owned &&
        map_data.type != mdata_t::MapType::town &&
        map_data.type != mdata_t::MapType::field &&
        map_data.type != mdata_t::MapType::shelter &&
        map_data.type != mdata_t::MapType::guild)
    {
        txt(i18n::s.get("core.ui.inv.cannot_use_cargo_items"),
            Message::only_once{true});
        snd("core.fail1");
        return false;
    }
    else
    {
        return true;
    }
}



int convertartifact(int item_index, int ignore_external_container)
{
    int f_at_m163 = 0;
    int tc_at_m163 = 0;
    std::string n_at_m163;
    if (!is_equipment(the_item_db[itemid2int(inv[item_index].id)]->category))
    {
        return item_index;
    }
    if (inv[item_index].quality != Quality::special)
    {
        return item_index;
    }
    if (inv[item_index].body_part != 0)
    {
        return item_index;
    }
    f_at_m163 = 0;
    for (int cnt = 0; cnt < ELONA_MAX_ITEMS; ++cnt)
    {
        if (ignore_external_container)
        {
            if (cnt >= ELONA_ITEM_ON_GROUND_INDEX)
            {
                break;
            }
        }
        if (cnt >= 520 && cnt < 5060)
        {
            continue;
        }
        tc_at_m163 = inv_getowner(cnt);
        if (tc_at_m163 != -1)
        {
            if (cdata[tc_at_m163].state() == Character::State::empty ||
                cdata[tc_at_m163].character_role == 13)
            {
                continue;
            }
        }
        if (inv[cnt].number() > 0)
        {
            if (inv[cnt].id == inv[item_index].id)
            {
                if (cnt != item_index)
                {
                    f_at_m163 = 1;
                    break;
                }
            }
        }
    }
    if (f_at_m163 == 0)
    {
        return item_index;
    }
    n_at_m163 = ""s + itemname(inv[item_index]);

    while (true)
    {
        flt(the_item_db[itemid2int(inv[item_index].id)]->level,
            Quality::miracle);
        flttypeminor = the_item_db[itemid2int(inv[item_index].id)]->subcategory;
        inv[item_index].remove();

        itemcreate(inv_getowner(item_index), 0, inv[item_index].position, 0);
        if (inv[item_index].quality != Quality::special)
        {
            break;
        }
    }

    txt(i18n::s.get("core.misc.artifact_regeneration", n_at_m163, inv[ci]));
    return item_index;
}



void damage_by_cursed_equipments()
{
    if (rnd(4) == 0)
    {
        damage_hp(
            cdata[cc],
            cdata[cc].hp * (5 + cdata[cc].curse_power / 5) / 100,
            -5);
        return;
    }
    if (map_data.type != mdata_t::MapType::world_map)
    {
        if (rnd(10 - clamp(cdata[cc].curse_power / 10, 0, 9)) == 0)
        {
            efid = 408;
            tc = cc;
            magic();
            return;
        }
    }
    if (rnd(10) == 0)
    {
        if (cdata[cc].gold > 0)
        {
            p = rnd_capped(cdata[cc].gold) / 100 + rnd(10) + 1;
            if (p > cdata[cc].gold)
            {
                p = cdata[cc].gold;
            }
            cdata[cc].gold -= p;
            if (is_in_fov(cdata[cc]))
            {
                txt(i18n::s.get("core.misc.curse.gold_stolen", cdata[cc]),
                    Message::color{ColorIndex::purple});
            }
            return;
        }
    }
}



void dipcursed(int item_index, int)
{
    if (the_item_db[itemid2int(inv[item_index].id)]->category ==
        ItemCategory::food)
    {
        if (inv[item_index].material == 35)
        {
            txt(i18n::s.get("core.action.dip.rots", inv[item_index]));
            inv[item_index].param3 = -1;
            inv[item_index].image = 336;
            cell_refresh(
                inv[item_index].position.x, inv[item_index].position.y);
            return;
        }
        else
        {
            txt(i18n::s.get("core.action.dip.unchanged", inv[item_index]));
            return;
        }
    }
    if (is_equipment(the_item_db[itemid2int(inv[item_index].id)]->category))
    {
        --inv[item_index].enhancement;
        txt(i18n::s.get("core.action.dip.rusts", inv[item_index]));
        if (inv_getowner(item_index) != -1)
        {
            chara_refresh(inv_getowner(item_index));
        }
        return;
    }
    txt(i18n::s.get("core.common.nothing_happens"));
}



int efstatusfix(int doomed, int cursed, int none, int blessed)
{
    switch (efstatus)
    {
    case CurseState::doomed: return doomed;
    case CurseState::cursed: return cursed;
    case CurseState::none: return none;
    case CurseState::blessed: return blessed;
    default: assert(0); return none;
    }
}



void equip_melee_weapon()
{
    attacknum = 0;
    for (int cnt = 0; cnt < 30; ++cnt)
    {
        body = 100 + cnt;
        if (cdata[cc].body_parts[cnt] / 10000 != 5)
        {
            continue;
        }
        if (cdata[cc].body_parts[cnt] % 10000 == 0)
        {
            continue;
        }
        const int cw = cdata[cc].body_parts[cnt] % 10000 - 1;
        if (inv[cw].dice_x == 0)
        {
            continue;
        }
        ++attacknum;
        if (cdata[cc].combat_style.two_hand())
        {
            if (inv[cw].weight >= 4000)
            {
                txt(i18n::s.get(
                    "core.action.equip.two_handed.fits_well", inv[cw]));
            }
            else
            {
                txt(i18n::s.get(
                    "core.action.equip.two_handed.too_light", inv[cw]));
            }
        }
        if (cdata[cc].combat_style.dual_wield())
        {
            if (attacknum == 1)
            {
                if (inv[cw].weight >= 4000)
                {
                    txt(i18n::s.get(
                        "core.action.equip.two_handed.too_heavy", inv[cw]));
                }
            }
            else if (inv[cw].weight > 1500)
            {
                txt(i18n::s.get(
                    "core.action.equip.two_handed.too_heavy_other_hand",
                    inv[cw]));
            }
        }
        if (cc == 0)
        {
            if (game_data.mount != 0)
            {
                if (inv[cw].weight >= 4000)
                {
                    txt(i18n::s.get(
                        "core.action.equip.two_handed.too_heavy_when_riding",
                        inv[cw]));
                }
            }
        }
    }
}



int gain_skills_by_geen_engineering()
{
    if (cdata[tc].splits() || cdata[tc].splits2())
    {
        return 0;
    }
    randomize(charaid2int(cdata[tc].id));
    int dbmax = 0;
    for (int cnt = 0; cnt < 100; ++cnt)
    {
        rtval = rnd(40) + 150;
        if (sdata(rtval, rc) == 0)
        {
            if (sdata(rtval, tc) > 0)
            {
                dblist(0, dbmax) = rtval;
                ++dbmax;
            }
        }
    }
    rtval(0) = dblist(0, 0);
    rtval(1) = -1;
    if (dbmax >= 2)
    {
        if (rnd(3) == 0)
        {
            for (int cnt = 1, cnt_end = cnt + (dbmax - 1); cnt < cnt_end; ++cnt)
            {
                if (dblist(0, cnt) != rtval)
                {
                    rtval(1) = dblist(0, cnt);
                    break;
                }
            }
        }
    }
    randomize();
    return dbmax;
}



int transplant_body_parts()
{
    int dbmax = 0;
    s(1) = chara_db_get_filter(cdata[tc].id);
    if (strutil::contains(s(1), u8"/man/"))
    {
        return -1;
    }
    if (cdata[tc].splits() || cdata[tc].splits2())
    {
        return -1;
    }
    rtval(1) = -1;
    for (int i = 0; i < 30; ++i)
    {
        if (cdata[rc].body_parts[i] == 0)
        {
            rtval(1) = i + 100;
        }
    }
    if (rtval(1) == -1)
    {
        return -1;
    }
    for (int cnt = 0; cnt < 30; ++cnt)
    {
        f = cdata[tc].body_parts[cnt] / 10000;
        if (f == 11 || f == 10 || f == 4)
        {
            continue;
        }
        if (f != 0)
        {
            dblist(0, dbmax) = f;
            ++dbmax;
        }
    }
    if (dbmax == 0)
    {
        return -1;
    }
    randomize(charaid2int(cdata[tc].id));
    for (int cnt = 0; cnt < 3; ++cnt)
    {
        rtval = dblist(0, rnd(dbmax));
        f = 0;
        for (int i = 0; i < 30; ++i)
        {
            if (cdata[rc].body_parts[i] == 0)
            {
                continue;
            }
            if (cdata[rc].body_parts[i] / 10000 == rtval)
            {
                f = 1;
            }
        }
        if (f)
        {
            break;
        }
    }
    if (f == 0)
    {
        randomize();
        return rtval(1);
    }
    DIM3(dblist, 2, 800);
    for (int i = 0; i < 30; ++i)
    {
        ++dblist(0, cdata[tc].body_parts[i] / 10000);
    }
    for (int cnt = 0; cnt < 25; ++cnt)
    {
        rtval = rnd(15) + 1;
        f = 0;
        for (int i = 0; i < 30; ++i)
        {
            if (cdata[rc].body_parts[i] / 10000 == rtval)
            {
                ++f;
            }
        }
        if (f < dblist(0, rtval))
        {
            f = -1;
            break;
        }
    }
    randomize();
    if (f == -1)
    {
        return rtval(1);
    }
    else
    {
        return -1;
    }
}

} // namespace elona
