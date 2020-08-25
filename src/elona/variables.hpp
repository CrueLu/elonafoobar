#pragma once

#include "elona.hpp"
#include "optional.hpp"
#include "position.hpp"
#include "random.hpp"


#ifdef ELONA_DEFINE_GLOBAL_INSTANCE
#define ELONA_EXTERN(declaration) declaration
#else
#define ELONA_EXTERN(declaration) extern declaration
#endif

namespace elona
{

struct Item;
struct Character;



ELONA_EXTERN(elona_vector1<int> ap);
ELONA_EXTERN(int nooracle);

// casino.cpp / casino_card.cpp
ELONA_EXTERN(std::string atxbg);
ELONA_EXTERN(std::string atxbgbk);
ELONA_EXTERN(elona_vector1<std::string> atxinfon);
ELONA_EXTERN(elona_vector1<int> fsetwear);
ELONA_EXTERN(elona_vector1<int> fsetitem);
ELONA_EXTERN(int txtadvscreenupdate);
ELONA_EXTERN(elona_vector2<int> matref);
ELONA_EXTERN(elona_vector1<std::string> matname);

// fov.cpp
ELONA_EXTERN(elona_vector2<int> route);
ELONA_EXTERN(int maxroute);

// building.cpp
ELONA_EXTERN(int tlocinitx);
ELONA_EXTERN(int tlocinity);
ELONA_EXTERN(elona_vector1<int> income);
ELONA_EXTERN(elona_vector2<std::string> rankn);
ELONA_EXTERN(int renewmulti);
ELONA_EXTERN(elona_vector2<int> bdref);

// ui.cpp
ELONA_EXTERN(int inf_clocky);
ELONA_EXTERN(int inf_msgh);
ELONA_EXTERN(int inf_ver);

// ui.cpp/menu.cpp
ELONA_EXTERN(int autoturn);
ELONA_EXTERN(int firstautoturn);
ELONA_EXTERN(int racount);
ELONA_EXTERN(elona_vector1<std::string> matdesc);
ELONA_EXTERN(std::string nquestdate);
ELONA_EXTERN(elona_vector2<int> podata);
ELONA_EXTERN(int refreshmode);
ELONA_EXTERN(bool quest_teleport);
ELONA_EXTERN(int gspdorg);
ELONA_EXTERN(int gspd);
ELONA_EXTERN(int scposval);
ELONA_EXTERN(int weatherbk);


// crafting.cpp / blending.cpp
ELONA_EXTERN(elona_vector1<int> rpsourcelist);
ELONA_EXTERN(int prodtype);
ELONA_EXTERN(elona_vector2<int> matspot);

// quest.cpp
ELONA_EXTERN(elona_vector1<int> fsetcollect);
ELONA_EXTERN(elona_vector1<int> asettown);
ELONA_EXTERN(elona_vector1<int> fsetdeliver);
ELONA_EXTERN(elona_vector1<int> fsetsupply);
ELONA_EXTERN(elona_vector1<int> fsetmagic);
ELONA_EXTERN(elona_vector1<int> fsetarmor);
ELONA_EXTERN(elona_vector1<int> fsetweapon);
ELONA_EXTERN(elona_vector1<int> fsetrewardsupply);

// map.cpp/mapgen.cpp
ELONA_EXTERN(int tile_trap);
ELONA_EXTERN(int tile_doorclosed4);
ELONA_EXTERN(int tile_dooropen);
ELONA_EXTERN(int tile_downlocked);
ELONA_EXTERN(int tile_re);
ELONA_EXTERN(int tile_plant);
ELONA_EXTERN(elona_vector1<int> isetgod);
ELONA_EXTERN(elona_vector1<int> isetcrop);
ELONA_EXTERN(int rdtry);

// food.cpp
ELONA_EXTERN(int enc);

// talk.cpp
ELONA_EXTERN(elona_vector2<std::string> actor);
ELONA_EXTERN(int current_actor_index);

// mef.cpp
ELONA_EXTERN(elona_vector2<int> mef);
ELONA_EXTERN(elona_vector2<int> mefsubref);

// map_cell.cpp
ELONA_EXTERN(int cellchara);
ELONA_EXTERN(int cellfeat);

// enchantment.cpp
ELONA_EXTERN(elona_vector2<int> encammoref);

// item.hpp/itemgen.hpp
ELONA_EXTERN(elona_vector1<int> isetbook);
ELONA_EXTERN(elona_vector1<int> moneybox);

// ai.cpp
ELONA_EXTERN(elona_vector1<int> isetthrowpotionminor);
ELONA_EXTERN(elona_vector1<int> isetthrowpotionmajor);
ELONA_EXTERN(elona_vector1<int> isetthrowpotiongreater);

// command.cpp
ELONA_EXTERN(int usermapid);
ELONA_EXTERN(int refx);
ELONA_EXTERN(int refy);
ELONA_EXTERN(int movx);
ELONA_EXTERN(int movy);

// dmgheal.cpp
ELONA_EXTERN(std::string ndeathcause);
ELONA_EXTERN(int catitem);
ELONA_EXTERN(int rollanatomy);
ELONA_EXTERN(int rtdmg);

// turn_sequence.cpp
ELONA_EXTERN(int tgloc);
ELONA_EXTERN(int dbg_skipevent);
ELONA_EXTERN(int findlocmode);
ELONA_EXTERN(int dirsub);
ELONA_EXTERN(int mousex);
ELONA_EXTERN(int mousey);

// initialize_map.cpp
ELONA_EXTERN(int noaggrorefresh);

ELONA_EXTERN(elona_vector1<int> arenaop);
ELONA_EXTERN(elona_vector1<int> commark);
ELONA_EXTERN(elona_vector1<int> cyclemax);
ELONA_EXTERN(elona_vector1<int> deck);
ELONA_EXTERN(elona_vector1<int> deliver);
ELONA_EXTERN(elona_vector1<int> dir);
ELONA_EXTERN(elona_vector1<int> dx);
ELONA_EXTERN(elona_vector1<int> feat);
ELONA_EXTERN(elona_vector1<int> fishanime);
ELONA_EXTERN(elona_vector1<int> fixeditemenc);
ELONA_EXTERN(elona_vector1<int> followerexist);
ELONA_EXTERN(elona_vector1<int> followerin);
ELONA_EXTERN(elona_vector1<int> fsetchest);
ELONA_EXTERN(elona_vector1<int> gdata);
ELONA_EXTERN(elona_vector1<int> genetemp);
ELONA_EXTERN(elona_vector1<int> giftvalue);
ELONA_EXTERN(elona_vector1<int> i);
ELONA_EXTERN(elona_vector1<int> ilight);
ELONA_EXTERN(elona_vector1<int> invcontainer);
ELONA_EXTERN(elona_vector1<int> invctrl);
ELONA_EXTERN(elona_vector1<int> invicon);
ELONA_EXTERN(elona_vector1<int> invmark);
ELONA_EXTERN(elona_vector1<int> iorgweight);
ELONA_EXTERN(elona_vector1<int> ipicref);
ELONA_EXTERN(elona_vector1<int> irandomname);
ELONA_EXTERN(elona_vector1<int> isetfruit);
ELONA_EXTERN(elona_vector1<int> mainskill);
ELONA_EXTERN(elona_vector1<int> mat);
ELONA_EXTERN(elona_vector1<int> mdata);
ELONA_EXTERN(elona_vector1<int> mdatatmp);
ELONA_EXTERN(elona_vector1<int> p);
ELONA_EXTERN(elona_vector1<int> recipememory);
ELONA_EXTERN(elona_vector1<int> reph);
ELONA_EXTERN(elona_vector1<int> repw);
ELONA_EXTERN(elona_vector1<int> rowactre);
ELONA_EXTERN(elona_vector1<int> rtval);
ELONA_EXTERN(elona_vector1<int> spact);
ELONA_EXTERN(elona_vector1<int> spell);
ELONA_EXTERN(elona_vector1<int> sx);
ELONA_EXTERN(elona_vector1<int> sy);
ELONA_EXTERN(elona_vector1<int> tile);
ELONA_EXTERN(elona_vector1<int> tile_default);
ELONA_EXTERN(elona_vector1<int> tile_fog);
ELONA_EXTERN(elona_vector1<int> tile_room);
ELONA_EXTERN(elona_vector1<int> tile_tunnel);
ELONA_EXTERN(elona_vector1<int> tile_wall);
ELONA_EXTERN(elona_vector1<int> trait);
ELONA_EXTERN(elona_vector1<int> traitref);
ELONA_EXTERN(elona_vector1<int> trate);
ELONA_EXTERN(elona_vector1<int> windowshadow);
ELONA_EXTERN(elona_vector1<int> x);
ELONA_EXTERN(elona_vector1<int> y);
ELONA_EXTERN(elona_vector1<std::string> ammoname);
ELONA_EXTERN(std::vector<std::string> artifactlocation);
ELONA_EXTERN(elona_vector1<std::string> booktitle);
ELONA_EXTERN(elona_vector1<std::string> buff);
ELONA_EXTERN(elona_vector1<std::string> buffboard);
ELONA_EXTERN(elona_vector1<std::string> description);
ELONA_EXTERN(elona_vector1<std::string> egominorn);
ELONA_EXTERN(elona_vector1<std::string> egoname);
ELONA_EXTERN(elona_vector1<std::string> filtern);
ELONA_EXTERN(elona_vector1<std::string> fltnrace);
ELONA_EXTERN(elona_vector1<std::string> ialphanameref);
ELONA_EXTERN(elona_vector1<std::string> iknownnameref);
ELONA_EXTERN(elona_vector1<std::string> inputlog);
ELONA_EXTERN(elona_vector1<std::string> invkey);
ELONA_EXTERN(elona_vector1<std::string> ioriginalnameref2);
ELONA_EXTERN(elona_vector1<std::string> ioriginalnameref);
ELONA_EXTERN(elona_vector1<std::string> key);
ELONA_EXTERN(elona_vector1<std::string> key_list);
ELONA_EXTERN(elona_vector1<std::string> key_select);
ELONA_EXTERN(elona_vector1<std::string> keylog);
ELONA_EXTERN(elona_vector1<std::string> mdatan);
ELONA_EXTERN(elona_vector1<std::string> newsbuff);
ELONA_EXTERN(elona_vector1<std::string> playerheader);
ELONA_EXTERN(elona_vector1<std::string> q);
ELONA_EXTERN(elona_vector1<std::string> qname);
ELONA_EXTERN(elona_vector1<std::string> s1);
ELONA_EXTERN(elona_vector1<std::string> s);
ELONA_EXTERN(elona_vector1<std::string> traitrefn2);
ELONA_EXTERN(elona_vector1<std::string> traitrefn);
ELONA_EXTERN(elona_vector2<int> adata);
ELONA_EXTERN(elona_vector2<int> breathlist);
ELONA_EXTERN(elona_vector2<int> c_col);
ELONA_EXTERN(elona_vector2<int> card);
ELONA_EXTERN(elona_vector2<int> cmapdata);
ELONA_EXTERN(elona_vector2<int> cycle);
ELONA_EXTERN(elona_vector2<int> dblist);
ELONA_EXTERN(elona_vector2<int> dirxy);
ELONA_EXTERN(elona_vector2<int> encprocref);
ELONA_EXTERN(elona_vector1<int> eqweapon1);
ELONA_EXTERN(elona_vector1<int> eqrange);
ELONA_EXTERN(elona_vector1<int> eqammo);
ELONA_EXTERN(elona_vector1<int> eqring1);
ELONA_EXTERN(elona_vector2<int> itemmemory);
ELONA_EXTERN(elona_vector2<int> list);
ELONA_EXTERN(elona_vector2<int> mapsync);
ELONA_EXTERN(elona_vector2<int> npcmemory);
ELONA_EXTERN(elona_vector2<int> pcc);
ELONA_EXTERN(elona_vector2<int> picfood);
ELONA_EXTERN(elona_vector2<int> qdata);
ELONA_EXTERN(elona_vector2<int> slight);
ELONA_EXTERN(elona_vector2<std::string> _melee);
ELONA_EXTERN(elona_vector2<std::string> listn);
ELONA_EXTERN(elona_vector2<std::string> mapnamerd);
ELONA_EXTERN(elona_vector3<int> bddata);
ELONA_EXTERN(elona_vector3<int> efmap);
ELONA_EXTERN(int ammoproc);
ELONA_EXTERN(int ammoprocbk);
ELONA_EXTERN(int area);
ELONA_EXTERN(int attacknum);
ELONA_EXTERN(int attackrange);
ELONA_EXTERN(int attackskill);
ELONA_EXTERN(int atxspot);
ELONA_EXTERN(int body);
ELONA_EXTERN(int bonus);
ELONA_EXTERN(int camera);
ELONA_EXTERN(int ccbk);
ELONA_EXTERN(int cellaccess);
ELONA_EXTERN(int chatesc);
ELONA_EXTERN(int chatteleport);
ELONA_EXTERN(int client);
ELONA_EXTERN(int cm);
ELONA_EXTERN(int cmbg);
ELONA_EXTERN(int cmsex);
ELONA_EXTERN(int cmshade);
ELONA_EXTERN(int creaturepack);
ELONA_EXTERN(int critical);
ELONA_EXTERN(int crop);
ELONA_EXTERN(int cs);
ELONA_EXTERN(int cs_bk2);
ELONA_EXTERN(int cs_bk);
ELONA_EXTERN(int cspecialeq);
ELONA_EXTERN(int cxinit);
ELONA_EXTERN(int cyinit);
ELONA_EXTERN(int damage);
ELONA_EXTERN(int dbg_revealmap);
ELONA_EXTERN(int dbid);
ELONA_EXTERN(int dice1);
ELONA_EXTERN(int dice2);
ELONA_EXTERN(int distance);
ELONA_EXTERN(int dmg);
ELONA_EXTERN(int dmgfix);
ELONA_EXTERN(double dmgmulti);
ELONA_EXTERN(int dy);
ELONA_EXTERN(int efcancel);
ELONA_EXTERN(int efid);
ELONA_EXTERN(int efp);
ELONA_EXTERN(int efsource);
ELONA_EXTERN(CurseState efstatus);
ELONA_EXTERN(int egolv);
ELONA_EXTERN(int ele);
ELONA_EXTERN(int elep);
ELONA_EXTERN(int en);
ELONA_EXTERN(int encounter);
ELONA_EXTERN(int encounterlv);
ELONA_EXTERN(int encounterref);
ELONA_EXTERN(int enemyteam);
ELONA_EXTERN(int eqmultiweapon);
ELONA_EXTERN(int eqtwohand);
ELONA_EXTERN(int equip);
ELONA_EXTERN(int exp);
ELONA_EXTERN(int f2);
ELONA_EXTERN(int f);
ELONA_EXTERN(int filtermax);
ELONA_EXTERN(int firstturn);
ELONA_EXTERN(int fish);
ELONA_EXTERN(int fishx);
ELONA_EXTERN(int fishy);
ELONA_EXTERN(Quality fixlv);
ELONA_EXTERN(int fixmaterial);
ELONA_EXTERN(int fixtransfermap);
ELONA_EXTERN(int fltselect);
ELONA_EXTERN(int flttypemajor);
ELONA_EXTERN(int flttypeminor);
ELONA_EXTERN(int foot);
ELONA_EXTERN(int grown);
ELONA_EXTERN(int hire);
ELONA_EXTERN(int homemapmode);
ELONA_EXTERN(int ichargelevel);
ELONA_EXTERN(IdentifyState idtresult);
ELONA_EXTERN(int in);
ELONA_EXTERN(int inf_maxlog);
ELONA_EXTERN(size_t inf_maxmsglen);
ELONA_EXTERN(int inf_msgspace);
ELONA_EXTERN(int inf_msgx);
ELONA_EXTERN(int inf_msgy);
ELONA_EXTERN(int inf_screenh);
ELONA_EXTERN(int inf_screenw);
ELONA_EXTERN(int inf_screenx);
ELONA_EXTERN(int inf_screeny);
ELONA_EXTERN(int inf_tiles);
ELONA_EXTERN(int inf_verh);
ELONA_EXTERN(int inf_very);
ELONA_EXTERN(int initeco);
ELONA_EXTERN(int initlv);
ELONA_EXTERN(int invally);
ELONA_EXTERN(int invfile);
ELONA_EXTERN(int invhead);
ELONA_EXTERN(int invrange);
ELONA_EXTERN(int invsc);
ELONA_EXTERN(int invsubroutine);
ELONA_EXTERN(int jp);
ELONA_EXTERN(bool key_escape);
ELONA_EXTERN(int keybd_attacking);
ELONA_EXTERN(int keybd_wait);
ELONA_EXTERN(int keyrange);
ELONA_EXTERN(int keywait);
ELONA_EXTERN(int lastctrl);
ELONA_EXTERN(int len);
ELONA_EXTERN(int levelexitby);
ELONA_EXTERN(int listmax);
ELONA_EXTERN(int lomiaseaster);
ELONA_EXTERN(int mapstartx);
ELONA_EXTERN(int mapstarty);
ELONA_EXTERN(int mapsubroutine);
ELONA_EXTERN(int mapupdate);
ELONA_EXTERN(int marry);
ELONA_EXTERN(int maxbreath);
ELONA_EXTERN(int maxitemid);
ELONA_EXTERN(int menucycle);
ELONA_EXTERN(int minlevel);
ELONA_EXTERN(int mode);
ELONA_EXTERN(int msync);
ELONA_EXTERN(int mtilefilecur);
ELONA_EXTERN(int nostack);
ELONA_EXTERN(int novoidlv);
ELONA_EXTERN(int nowindowanime);
ELONA_EXTERN(int objfix);
ELONA_EXTERN(int objlv);
ELONA_EXTERN(int obvious);
ELONA_EXTERN(int orgdmg);
ELONA_EXTERN(int page);
ELONA_EXTERN(int page_bk);
ELONA_EXTERN(int pagemax);
ELONA_EXTERN(int pagesize);
ELONA_EXTERN(int pcattacker);
ELONA_EXTERN(int petarenawin);
ELONA_EXTERN(int potionspill);
ELONA_EXTERN(int potionthrow);
ELONA_EXTERN(int promptx);
ELONA_EXTERN(int prompty);
ELONA_EXTERN(int quickpage);
ELONA_EXTERN(int r2);
ELONA_EXTERN(int r);
ELONA_EXTERN(int range_);
ELONA_EXTERN(int rapidmagic);
ELONA_EXTERN(int reftype);
ELONA_EXTERN(int reftypeminor);
ELONA_EXTERN(int returnfromidentify);
ELONA_EXTERN(int rq);
ELONA_EXTERN(int running);
ELONA_EXTERN(int sc);
ELONA_EXTERN(int scene_cut);
ELONA_EXTERN(int sceneid);
ELONA_EXTERN(int scenemode);
ELONA_EXTERN(int screenupdate);
ELONA_EXTERN(int scroll);
ELONA_EXTERN(int scrollanime);
ELONA_EXTERN(int scrollp);
ELONA_EXTERN(int scrturn);
ELONA_EXTERN(int scx);
ELONA_EXTERN(int scxbk);
ELONA_EXTERN(int scy);
ELONA_EXTERN(int scybk);
ELONA_EXTERN(int shoptrade);
ELONA_EXTERN(int size);
ELONA_EXTERN(int sizefix);
ELONA_EXTERN(int sound);
ELONA_EXTERN(int strsize);
ELONA_EXTERN(int supply);
ELONA_EXTERN(int sxfix);
ELONA_EXTERN(int syfix);
ELONA_EXTERN(int t);
ELONA_EXTERN(int tail);
ELONA_EXTERN(int tcbk);
ELONA_EXTERN(int tg);
ELONA_EXTERN(int tile_doorclosed);
ELONA_EXTERN(int tile_downstairs);
ELONA_EXTERN(int tile_hidden);
ELONA_EXTERN(int tile_upstairs);
ELONA_EXTERN(int time_begin);
ELONA_EXTERN(int time_warn);
ELONA_EXTERN(int tlocx);
ELONA_EXTERN(int tlocy);
ELONA_EXTERN(int tx);
ELONA_EXTERN(int txt3rd);
ELONA_EXTERN(int ty);
ELONA_EXTERN(int vfix);
ELONA_EXTERN(int voidlv);
ELONA_EXTERN(int windowh);
ELONA_EXTERN(int windoww);
ELONA_EXTERN(int wh);
ELONA_EXTERN(int ww);
ELONA_EXTERN(int wx);
ELONA_EXTERN(int wy);
ELONA_EXTERN(int x1);
ELONA_EXTERN(int x2);
ELONA_EXTERN(int y1);
ELONA_EXTERN(int y2);
ELONA_EXTERN(std::string defload);
ELONA_EXTERN(std::string fmapfile);
ELONA_EXTERN(std::string geneuse);
ELONA_EXTERN(std::string mid);
ELONA_EXTERN(std::string playerid);
ELONA_EXTERN(std::string strhint1);
ELONA_EXTERN(std::string strhint2);
ELONA_EXTERN(std::string strhint3);
ELONA_EXTERN(std::string strhint3b);
ELONA_EXTERN(std::string strhint4);
ELONA_EXTERN(std::string strhint5);
ELONA_EXTERN(std::string strhint5b);
ELONA_EXTERN(std::string strhint7);

ELONA_EXTERN(optional<int> chatval_unique_chara_id);

} // namespace elona
