local Data = ELONA.require("core.Data")

Data.define_prototype("class")
Data.add(
   "core.class",
   {
      warrior = {
         ordering = 10010,
         item_type = 1,
         is_extra = false,
         equipment_type = 1,
         skills = {
            ["core.stat_strength"] = 10,
            ["core.stat_constitution"] = 10,
            ["core.stat_dexterity"] = 2,
            ["core.stat_perception"] = 0,
            ["core.stat_learning"] = 0,
            ["core.stat_will"] = 3,
            ["core.stat_magic"] = 0,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 0,
            ["core.long_sword"] = 6,
            ["core.short_sword"] = 4,
            ["core.blunt"] = 6,
            ["core.axe"] = 6,
            ["core.polearm"] = 4,
            ["core.scythe"] = 5,
            ["core.two_hand"] = 6,
            ["core.tactics"] = 4,
            ["core.evasion"] = 5,
            ["core.healing"] = 5,
            ["core.medium_armor"] = 4,
            ["core.heavy_armor"] = 4,
            ["core.shield"] = 5,
         },
      },
      thief = {
         ordering = 10020,
         item_type = 7,
         is_extra = false,
         equipment_type = 7,
         skills = {
            ["core.stat_strength"] = 4,
            ["core.stat_constitution"] = 4,
            ["core.stat_dexterity"] = 8,
            ["core.stat_perception"] = 5,
            ["core.stat_learning"] = 4,
            ["core.stat_will"] = 0,
            ["core.stat_magic"] = 0,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 0,
            ["core.short_sword"] = 4,
            ["core.long_sword"] = 3,
            ["core.bow"] = 3,
            ["core.evasion"] = 4,
            ["core.dual_wield"] = 4,
            ["core.tactics"] = 3,
            ["core.negotiation"] = 4,
            ["core.pickpocket"] = 4,
            ["core.magic_device"] = 3,
            ["core.medium_armor"] = 4,
         },
      },
      wizard = {
         ordering = 10030,
         item_type = 2,
         is_extra = false,
         equipment_type = 2,
         skills = {
            ["core.stat_strength"] = 0,
            ["core.stat_constitution"] = 0,
            ["core.stat_dexterity"] = 0,
            ["core.stat_perception"] = 4,
            ["core.stat_learning"] = 3,
            ["core.stat_will"] = 8,
            ["core.stat_magic"] = 10,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 0,
            ["core.stave"] = 3,
            ["core.short_sword"] = 2,
            ["core.literacy"] = 6,
            ["core.memorization"] = 3,
            ["core.magic_capacity"] = 6,
            ["core.magic_device"] = 5,
            ["core.light_armor"] = 4,
            ["core.alchemy"] = 4,
            ["core.casting"] = 5,
         },
      },
      farmer = {
         ordering = 10040,
         item_type = 1,
         is_extra = false,
         equipment_type = 1,
         skills = {
            ["core.stat_strength"] = 5,
            ["core.stat_constitution"] = 5,
            ["core.stat_dexterity"] = 2,
            ["core.stat_perception"] = 0,
            ["core.stat_learning"] = 8,
            ["core.stat_will"] = 5,
            ["core.stat_magic"] = 0,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 0,
            ["core.polearm"] = 4,
            ["core.scythe"] = 3,
            ["core.evasion"] = 3,
            ["core.negotiation"] = 5,
            ["core.cooking"] = 6,
            ["core.anatomy"] = 7,
            ["core.healing"] = 5,
            ["core.gardening"] = 5,
            ["core.tailoring"] = 5,
         },
      },
      predator = {
         ordering = 20010,
         item_type = 0,
         is_extra = true,
         equipment_type = 0,
         skills = {
            ["core.stat_strength"] = 8,
            ["core.stat_constitution"] = 11,
            ["core.stat_dexterity"] = 8,
            ["core.stat_perception"] = 0,
            ["core.stat_learning"] = 0,
            ["core.stat_will"] = 0,
            ["core.stat_magic"] = 0,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 10,
            ["core.tactics"] = 4,
         },
      },
      archer = {
         ordering = 10050,
         item_type = 3,
         is_extra = false,
         equipment_type = 3,
         skills = {
            ["core.stat_strength"] = 6,
            ["core.stat_constitution"] = 4,
            ["core.stat_dexterity"] = 8,
            ["core.stat_perception"] = 5,
            ["core.stat_learning"] = 2,
            ["core.stat_will"] = 0,
            ["core.stat_magic"] = 0,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 0,
            ["core.bow"] = 5,
            ["core.crossbow"] = 5,
            ["core.short_sword"] = 4,
            ["core.axe"] = 3,
            ["core.evasion"] = 5,
            ["core.magic_device"] = 3,
            ["core.medium_armor"] = 3,
            ["core.tailoring"] = 4,
            ["core.riding"] = 4,
            ["core.marksman"] = 3,
         },
      },
      warmage = {
         ordering = 10060,
         item_type = 5,
         is_extra = false,
         equipment_type = 5,
         skills = {
            ["core.stat_strength"] = 6,
            ["core.stat_constitution"] = 6,
            ["core.stat_dexterity"] = 2,
            ["core.stat_perception"] = 0,
            ["core.stat_learning"] = 0,
            ["core.stat_will"] = 4,
            ["core.stat_magic"] = 7,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 0,
            ["core.long_sword"] = 4,
            ["core.short_sword"] = 3,
            ["core.evasion"] = 3,
            ["core.literacy"] = 4,
            ["core.magic_capacity"] = 3,
            ["core.magic_device"] = 5,
            ["core.medium_armor"] = 4,
            ["core.heavy_armor"] = 4,
            ["core.casting"] = 4,
         },
      },
      tourist = {
         ordering = 10070,
         item_type = 0,
         is_extra = false,
         equipment_type = 0,
         skills = {
            ["core.stat_strength"] = 0,
            ["core.stat_constitution"] = 0,
            ["core.stat_dexterity"] = 0,
            ["core.stat_perception"] = 0,
            ["core.stat_learning"] = 0,
            ["core.stat_will"] = 0,
            ["core.stat_magic"] = 0,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 0,
            ["core.fishing"] = 5,
            ["core.traveling"] = 3,
         },
      },
      pianist = {
         ordering = 10080,
         item_type = 3,
         is_extra = false,
         equipment_type = 3,
         skills = {
            ["core.stat_strength"] = 6,
            ["core.stat_constitution"] = 0,
            ["core.stat_dexterity"] = 4,
            ["core.stat_perception"] = 5,
            ["core.stat_learning"] = 6,
            ["core.stat_will"] = 0,
            ["core.stat_magic"] = 4,
            ["core.stat_charisma"] = 8,
            ["core.stat_speed"] = 0,
            ["core.performer"] = 6,
            ["core.weight_lifting"] = 19,
            ["core.literacy"] = 4,
            ["core.memorization"] = 6,
            ["core.magic_device"] = 6,
            ["core.jeweler"] = 5,
            ["core.light_armor"] = 4,
            ["core.riding"] = 3,
         },
      },
      gunner = {
         ordering = 20020,
         item_type = 4,
         is_extra = true,
         equipment_type = 4,
         skills = {
            ["core.stat_strength"] = 0,
            ["core.stat_constitution"] = 2,
            ["core.stat_dexterity"] = 5,
            ["core.stat_perception"] = 8,
            ["core.stat_learning"] = 5,
            ["core.stat_will"] = 4,
            ["core.stat_magic"] = 3,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 0,
            ["core.firearm"] = 5,
            ["core.evasion"] = 4,
            ["core.literacy"] = 3,
            ["core.healing"] = 4,
            ["core.marksman"] = 3,
         },
      },
      priest = {
         ordering = 10090,
         item_type = 6,
         is_extra = false,
         equipment_type = 6,
         skills = {
            ["core.stat_strength"] = 2,
            ["core.stat_constitution"] = 2,
            ["core.stat_dexterity"] = 0,
            ["core.stat_perception"] = 2,
            ["core.stat_learning"] = 2,
            ["core.stat_will"] = 10,
            ["core.stat_magic"] = 7,
            ["core.stat_charisma"] = 2,
            ["core.stat_speed"] = 0,
            ["core.blunt"] = 3,
            ["core.shield"] = 3,
            ["core.literacy"] = 5,
            ["core.healing"] = 5,
            ["core.magic_device"] = 5,
            ["core.medium_armor"] = 3,
            ["core.heavy_armor"] = 4,
            ["core.faith"] = 5,
            ["core.casting"] = 5,
         },
      },
      claymore = {
         ordering = 10100,
         item_type = 8,
         is_extra = false,
         equipment_type = 8,
         skills = {
            ["core.stat_strength"] = 9,
            ["core.stat_constitution"] = 3,
            ["core.stat_dexterity"] = 7,
            ["core.stat_perception"] = 6,
            ["core.stat_learning"] = 0,
            ["core.stat_will"] = 0,
            ["core.stat_magic"] = 4,
            ["core.stat_charisma"] = 0,
            ["core.stat_speed"] = 0,
            ["core.long_sword"] = 6,
            ["core.two_hand"] = 7,
            ["core.tactics"] = 5,
            ["core.evasion"] = 7,
            ["core.greater_evasion"] = 4,
            ["core.healing"] = 6,
            ["core.light_armor"] = 5,
            ["core.literacy"] = 4,
         },
      },
      -- For debug
      debugger = {
         ordering = 0,
         item_type = 1,
         is_extra = true,
         equipment_type = 1,
         skills = {
            ["core.stat_strength"] = 50,
            ["core.stat_constitution"] = 50,
            ["core.stat_dexterity"] = 50,
            ["core.stat_perception"] = 50,
            ["core.stat_learning"] = 50,
            ["core.stat_will"] = 50,
            ["core.stat_magic"] = 50,
            ["core.stat_charisma"] = 50,
            ["core.stat_speed"] = 50,
            ["core.long_sword"] = 50,
            ["core.short_sword"] = 50,
            ["core.axe"] = 50,
            ["core.blunt"] = 50,
            ["core.polearm"] = 50,
            ["core.stave"] = 50,
            ["core.scythe"] = 50,
            ["core.bow"] = 50,
            ["core.crossbow"] = 50,
            ["core.firearm"] = 50,
            ["core.literacy"] = 50,
            ["core.tactics"] = 50,
            ["core.weight_lifting"] = 509,
            ["core.healing"] = 50,
            ["core.negotiation"] = 50,
            ["core.anatomy"] = 50,
            ["core.magic_capacity"] = 50,
            ["core.memorization"] = 50,
            ["core.dual_wield"] = 50,
            ["core.two_hand"] = 50,
            ["core.shield"] = 50,
            ["core.heavy_armor"] = 50,
            ["core.medium_armor"] = 50,
            ["core.light_armor"] = 50,
            ["core.casting"] = 50,
            ["core.evasion"] = 50,
            ["core.magic_device"] = 50,
            ["core.tailoring"] = 50,
            ["core.alchemy"] = 50,
            ["core.jeweler"] = 50,
            ["core.gardening"] = 50,
            ["core.faith"] = 50,
            ["core.traveling"] = 50,
            ["core.performer"] = 50,
            ["core.cooking"] = 50,
            ["core.fishing"] = 50,
            ["core.greater_evasion"] = 50,
            ["core.marksman"] = 50,
            ["core.pickpocket"] = 50,
            ["core.riding"] = 50,
            ["core.lock_picking"] = 50,
         },
      },
   }
)
