#pragma once



namespace elona
{

enum class CharaId : int
{
    none = 0,
    bug = 0,
    shopkeeper = 1,
    zeome = 2,
    putit = 3,
    red_putit = 4,
    rabbit = 5,
    snail = 6,
    fallen_soldier = 7,
    mercenary = 8,
    beggar = 9,
    wild_sheep = 10,
    flying_frog = 11,
    gangster = 12,
    kobold = 13,
    centipede = 14,
    mushroom = 15,
    citizen = 16,
    orc = 17,
    warrior_of_elea = 18,
    mandrake = 19,
    orc_warrior = 20,
    zombie = 21,
    beetle = 22,
    orphe = 23,
    wizard_of_elea = 24,
    goda = 25,
    mad_scientist = 26,
    isca = 27,
    whom_dwell_in_the_vanity = 28,
    loyter = 29,
    miches = 30,
    shena = 31,
    dungeon_cleaner = 32,
    larnneire = 33,
    lomias = 34,
    town_child = 35,
    old_person = 36,
    at = 37,
    elder = 38,
    citizen2 = 39,
    trainer = 40,
    wizard = 41,
    bat = 42,
    vampire_bat = 43,
    dragon_bat = 44,
    fire_ent = 45,
    ice_ent = 46,
    lich = 47,
    master_lich = 48,
    demi_lich = 49,
    hound = 50,
    fire_hound = 51,
    ice_hound = 52,
    lightning_hound = 53,
    dark_hound = 54,
    illusion_hound = 55,
    nerve_hound = 56,
    poison_hound = 57,
    sound_hound = 58,
    nether_hound = 59,
    chaos_hound = 60,
    giant_squirrel = 61,
    killer_squirrel = 62,
    grudge = 63,
    hungry_demon = 64,
    electric_cloud = 65,
    chaos_cloud = 66,
    floating_eye = 67,
    wyvern = 68,
    informer = 69,
    bartender = 70,
    sailor = 71,
    captain = 72,
    arena_master = 73,
    healer = 74,
    warrior = 75,
    guard_port_kapul = 76,
    guard = 77,
    puppet = 78,
    stersha = 79,
    xabi = 80,
    wasp = 81,
    red_wasp = 82,
    cyclops = 83,
    titan = 84,
    imp = 85,
    nether_imp = 86,
    chaos_imp = 87,
    hand_of_the_dead = 88,
    hand_of_the_chaos = 89,
    hand_of_the_murderer = 90,
    ghost = 91,
    nymph = 92,
    man_eater_flower = 93,
    chaos_flower = 94,
    cobra = 95,
    king_cobra = 96,
    fire_drake = 97,
    ice_drake = 98,
    lesser_mummy = 99,
    mummy = 100,
    greater_mummy = 101,
    goblin = 102,
    goblin_warrior = 103,
    goblin_shaman = 104,
    goblin_wizard = 105,
    red_baptist = 106,
    blue_baptist = 107,
    brown_bear = 108,
    grizzly = 109,
    living_armor = 110,
    steel_mass = 111,
    golden_armor = 112,
    death_armor = 113,
    medusa = 114,
    euryale = 115,
    stheno = 116,
    cupid_of_love = 117,
    lesser_phantom = 118,
    harpy = 119,
    green_dragon = 120,
    red_dragon = 121,
    white_dragon = 122,
    elec_dragon = 123,
    nether_dragon = 124,
    chaos_dragon = 125,
    cerberus = 126,
    spider = 127,
    black_widow = 128,
    paralyzer = 129,
    tarantula = 130,
    blood_spider = 131,
    wooden_golem = 132,
    stone_golem = 133,
    steel_golem = 134,
    golden_golem = 135,
    mithril_golem = 136,
    sky_golem = 137,
    adamantium_golem = 138,
    slan = 139,
    vesda = 140,
    issizzle = 141,
    erystia = 142,
    wynan = 143,
    quruiza = 144,
    corgon = 145,
    karam = 146,
    fire_crab = 147,
    fire_centipede = 148,
    cultist_of_fire = 149,
    skeleton_warrior = 150,
    skeleton_berserker = 151,
    missionary_of_darkness = 152,
    pawn = 153,
    rook = 154,
    bishop = 155,
    knight = 156,
    queen = 157,
    king = 158,
    mercenary_warrior = 159,
    mercenary_archer = 160,
    mercenary_wizard = 161,
    yerles_machine_infantry = 162,
    rock_thrower = 163,
    cat = 164,
    dog = 165,
    little_girl = 166,
    rat = 167,
    hermit_crab = 168,
    slime = 169,
    public_performer = 170,
    citizen_of_cyber_dome = 171,
    citizen_of_cyber_dome2 = 172,
    sales_person = 173,
    punk = 174,
    frisia = 175,
    younger_sister = 176,
    utima = 177,
    azzrssil = 178,
    master_of_pet_arena = 179,
    twintail = 180,
    silver_wolf = 181,
    nurse = 182,
    rich_person = 183,
    noble_child = 184,
    tourist = 185,
    blade = 186,
    blade_alpha = 187,
    blade_omega = 188,
    deformed_eye = 189,
    impure_eye = 190,
    wisp = 191,
    hedgehog = 192,
    shining_hedgehog = 193,
    acid_slime = 194,
    chicken = 195,
    pumpkin = 196,
    greater_pumpkin = 197,
    halloween_nightmare = 198,
    stalker = 199,
    shadow_stalker = 200,
    puppy = 201,
    ebon = 202,
    moyer = 203,
    palmian_elite_soldier = 204,
    maid = 205,
    nun = 206,
    ebon2 = 207,
    garok = 208,
    miral = 209,
    younger_cat_sister = 210,
    young_lady = 211,
    test_subject = 212,
    gwen = 213,
    thief = 214,
    robber = 215,
    great_race_of_yith = 216,
    master_thief = 217,
    shub_niggurath = 218,
    gagu = 219,
    spiral_king = 220,
    pael = 221,
    lily = 222,
    raphael = 223,
    ainc = 224,
    poppy = 225,
    rilian = 226,
    tam = 227,
    carbuncle = 228,
    lion = 229,
    cacy = 230,
    gilbert = 231,
    yerles_self_propelled_gun = 232,
    juere_infantry = 233,
    yerles_elite_machine_infantry = 234,
    juere_swordman = 235,
    yeek = 236,
    kamikaze_yeek = 237,
    yeek_warrior = 238,
    hot_spring_maniac = 239,
    master_yeek = 240,
    yeek_archer = 241,
    rodlob = 242,
    arnord = 243,
    kamikaze_samurai = 244,
    bomb_rock = 245,
    silver_cat = 246,
    mia = 247,
    tyrannosaurus = 248,
    younger_sister_of_mansion = 249,
    fairy = 250,
    troll = 251,
    renton = 252,
    marks = 253,
    ancient_coffin = 254,
    scorpion = 255,
    king_scorpion = 256,
    tuwen = 257,
    iron_maiden = 258,
    noel = 259,
    black_cat = 260,
    cute_fairy = 261,
    android = 262,
    black_angel = 263,
    exile = 264,
    golden_knight = 265,
    defender = 266,
    lame_horse = 267,
    yowyn_horse = 268,
    farmer = 269,
    sister = 270,
    rogue = 271,
    artist = 272,
    miner = 273,
    noble = 274,
    noyel_horse = 275,
    wild_horse = 276,
    wild_horse2 = 277,
    mutant = 278,
    icolle = 279,
    balzak = 280,
    lizard_man = 281,
    minotaur = 282,
    spore_mushroom = 283,
    chaos_mushroom = 284,
    blue_bubble = 285,
    bubble = 286,
    mass_monster = 287,
    revlus = 288,
    mage_guild_member = 289,
    lexus = 290,
    fray = 291,
    sin = 292,
    thief_guild_member = 293,
    abyss = 294,
    fighter_guild_member = 295,
    minotaur_magician = 296,
    doria = 297,
    minotaur_boxer = 298,
    minotaur_king = 299,
    ungaga = 300,
    conery = 301,
    rogue_boss = 302,
    rogue_warrior = 303,
    rogue_archer = 304,
    rogue_wizard = 305,
    lulwy = 306,
    executioner = 307,
    messenger_of_death = 308,
    asura = 309,
    mitra = 310,
    varuna = 311,
    hungry_sea_lion = 312,
    super_hungry_sea_lion = 313,
    death_gaze = 314,
    chaos_eye = 315,
    mad_gaze = 316,
    silver_eyed_witch = 317,
    big_daddy = 318,
    little_sister = 319,
    cleaner = 320,
    hard_gay = 321,
    strange_scientist = 322,
    shade = 323,
    quickling = 324,
    quickling_archer = 325,
    bard = 326,
    cube = 327,
    silver_bell = 328,
    gold_bell = 329,
    alien = 330,
    ehekatl = 331,
    stray_cat = 332,
    guild_trainer = 333,
    mysterious_producer = 334,
    prostitute = 335,
    god_inside_ehekatl = 336,
    prisoner = 337,
    opatos = 338,
    kumiromi = 339,
    _test = 340,
    mine_dog = 341,
    mani = 342,
    user = 343,
    mammoth = 344,
    kaneda_bike = 345,
    cub = 346,
    fanatic = 347,
    part_time_worker = 348,
    holy_beast = 349,
    festival_tourist = 350,
    the_leopard_warrior = 351,
    silvia = 352,
    caravan_master = 353,
};



inline int charaid2int(CharaId x)
{
    return static_cast<int>(x);
}



inline CharaId int2charaid(int x)
{
    return static_cast<CharaId>(x);
}

} // namespace elona
