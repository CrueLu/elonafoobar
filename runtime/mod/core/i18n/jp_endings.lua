local I18N = ELONA.require("core.I18N")
local Rand = ELONA.require("core.Rand")

local marks = {"。", "？", "！", ""}
local endings = {
   yoro = {
      {{"よろしくお願いします", "どうぞ、よろしくです"},
         {"よろしくお願いしますわ", "よろしくです"}},
      {{"よろしく頼むぜ", "よろしくな"},
         {"よろしくね", "よろしくな"}},
      {{"よろしくね", "よろしくお願いするよ"},
         {"よろしくねっ", "よろしく〜"}},
      {{"よろしく…", "今後とも、よろしく…"},
         {"よろしくね…", "よろ…"}},
      {{"よろしく頼もう", "よろしく頼むぞよ"},
         {"よろしく頼むぞよ", "よろしく頼むぞな"}},
      {{"よしなに", "よろしく頼むでござる"},
         {"よろしくでござりまする", "どうぞよしなに"}},
      {{"よろしくッス"},
         {"よろしくにゃの"}},
   },
   dozo = {
      {{"はい、どうぞ", "お待ちどうさまです"},
         {"はい、どうぞ", "注文の品ですわ"}},
      {{"ほらよ", "ほれ"},
         {"ほら", "待たせたね"}},
      {{"はい、お待ち", "さあ、どうぞ"},
         {"さあ、どうぞ", "お待ちどうさま"}},
      {{"ほら…", "待たせたな…"},
         {"はい…", "どうぞ…"}},
      {{"ほうれ", "ほれ、受け取りたまえ"},
         {"ほれ、受け取るが良い", "ほれ、待たせたのう"}},
      {{"お待たせ申した", "待たせたでござる"},
         {"お待たせ致しました", "ささ、どうぞ"}},
      {{"お待たせッス"},
         {"お待たせにゃん"}},
   },
   thanks = {
      {{"感謝します", "ありがとうございます"},
         {"感謝します", "ありがとうございます"}},
      {{"ありがとよ", "ありがたい"},
         {"礼を言うよ", "ありがたいね"}},
      {{"ありがとう", "感謝するよ"},
         {"ありがとう〜", "感謝するわ"}},
      {{"礼を言う…", "感謝する…"},
         {"ありがと…", "礼を言うわ…"}},
      {{"礼を申すぞ", "感謝してつかわす"},
         {"くるしゅうない", "礼をいってつかわす"}},
      {{"かたじけない", "恩に着る"},
         {"ありがたや", "お礼申し上げます"}},
      {{"アザーッス"},
         {"にゃりーん"}},
   },
   rob = {
      {{"悪いことは言わない。おやめなさい", "止めてください。きっと後悔しますよ"},
         {"止めてくださいませ", "こういう時のために、傭兵に金をかけているのです"}},
      {{"なんだ、貴様賊だったのか", "馬鹿な奴だ。後になって謝っても遅いぞ"},
         {"ふん、返り討ちにしてくれるよ", "ごろつき風情に何ができる"}},
      {{"おい、傭兵さんたち、このごろつきを追い払ってくれ", "馬鹿な真似をするな。こっちには屈強の傭兵がいるんだぞ"},
         {"やめて", "傭兵さんたち〜出番ですよ"}},
      {{"甘く見られたものだ…", "この護衛の数が見えないのか…"},
         {"おやめ…", "愚かな試みよ…"}},
      {{"なんたる無礼者か", "ほほほ、こやつめ"},
         {"下賤の者どもの分際で", "ほほほ、殺してあげなさい"}},
      {{"何をするでござるか"},
         {"ご無体な", "まあ、お戯れが過ぎますわ"}},
      {{"見損なったッス"},
         {"にゃりーん"}},
   },
   ka = {
      {{"ですか"},
         {"ですか"}},
      {{"かよ", "か"},
         {"かい"}},
      {{"かい", "なの"},
         {"なの"}},
      {{"か…", "かよ…"},
         {"なの…"}},
      {{"かのう", "であるか"},
         {"であるか"}},
      {{"でござるか"},
         {"でござりまするか"}},
      {{"ッスか"},
         {"かにゃ", "かニャン"}},
   },
   da = {
      {{"です", "ですね"},
         {"ですわ", "です"}},
      {{"だぜ", "だ"},
         {"ね", "よ"}},
      {{"だよ"},
         {"だわ", "よ"}},
      {{"だ…", "さ…"},
         {"よ…", "ね…"}},
      {{"じゃ", "でおじゃる"},
         {"じゃ", "でおじゃるぞ"}},
      {{"でござる", "でござるよ"},
         {"でござりまする"}},
      {{"ッス"},
         {"みゃん", "ミャ"}},
   },
   nda = {
      {{"のです", "んです"},
         {"のですわ", "のです"}},
      {{"", "んだ"},
         {"の"}},
      {{"んだよ", "んだ"},
         {"わ", "のよ"}},
      {{"…", "んだ…"},
         {"の…", "わ…"}},
      {{"のじゃ", "のだぞよ"},
         {"のじゃわ", "のだぞよ"}},
      {{"のでござる"},
         {"のでございます"}},
      {{"んだッス"},
         {"のニャ", "のにゃん"}},
   },
   noka = {
      {{"のですか", "んですか"},
         {"のですか", "んですか"}},
      {{"のか", "のだな"},
         {"の", "のかい"}},
      {{"のかい", "の"},
         {"の"}},
      {{"のか…"},
         {"の…"}},
      {{"のかのう", "のだな"},
         {"のかね", "のだな"}},
      {{"のでござるか"},
         {"のでございます"}},
      {{"のッスか"},
         {"にゃんか", "ニャン"}},
   },
   kana = {
      {{"でしょうか", "ですか"},
         {"かしら", "でしょう"}},
      {{"か", "かい"},
         {"か", "かい"}},
      {{"かな", "かなぁ"},
         {"かな", "かなー"}},
      {{"かな…", "か…"},
         {"かな…", "か…"}},
      {{"かのう", "かの"},
         {"かのう", "かの"}},
      {{"でござるか"},
         {"でございますか"}},
      {{"ッスか"},
         {"かにゃん", "かニャ"}},
   },
   kimi = {
      {{"貴方"},
         {"貴方"}},
      {{"お前"},
         {"お前"}},
      {{"君"},
         {"君"}},
      {{"君"},
         {"君"}},
      {{"お主"},
         {"お主"}},
      {{"そこもと"},
         {"そなた様"}},
      {{"アンタ"},
         {"あにゃた"}},
   },
   ru = {
      {{"ます", "ますよ"},
         {"ますわ", "ますの"}},
      {{"るぜ", "るぞ"},
         {"るわ", "るよ"}},
      {{"るよ", "るね"},
         {"るの", "るわ"}},
      {{"る…", "るが…"},
         {"る…", "るわ…"}},
      {{"るぞよ", "るぞ"},
         {"るぞよ", "るぞ"}},
      {{"るでござる", "るでござるよ"},
         {"るのでございます"}},
      {{"るッス"},
         {"るのニャ", "るにゃん"}},
   },
   tanomu = {
      {{"お願いします", "頼みます"},
         {"お願いしますわ", "頼みますわ"}},
      {{"頼む", "頼むな"},
         {"頼むよ", "頼む"}},
      {{"頼むね", "頼むよ"},
         {"頼むわ", "頼むね"}},
      {{"頼む…", "頼むぞ…"},
         {"頼むわ…", "頼むよ…"}},
      {{"頼むぞよ"},
         {"頼むぞよ"}},
      {{"頼み申す", "頼むでござる"},
         {"お頼み申し上げます"}},
      {{"頼むッス"},
         {"おねがいにゃ", "おねがいニャン"}},
   },
   ore = {
      {{"私"},
         {"私"}},
      {{"俺"},
         {"あたし"}},
      {{"僕"},
         {"わたし"}},
      {{"自分"},
         {"自分"}},
      {{"麻呂"},
         {"わらわ"}},
      {{"拙者"},
         {"手前"}},
      {{"あっし"},
         {"みゅー"}},
   },
   ga = {
      {{"ですが", "ですけど"},
         {"ですが", "ですけど"}},
      {{"が", "がな"},
         {"が"}},
      {{"けど", "が"},
         {"が", "けど"}},
      {{"が…", "けど…"},
         {"が…", "けど…"}},
      {{"であるが"},
         {"であるが"}},
      {{"でござるが"},
         {"でございますが"}},
      {{"ッスけど", "ッスが"},
         {"ニャけど", "にゃが"}},
   },
   dana = {
      {{"ですね"},
         {"ですわね", "ですね"}},
      {{"だな"},
         {"だね", "ね"}},
      {{"だね"},
         {"ね"}},
      {{"だな…"},
         {"だね…", "ね…"}},
      {{"であるな"},
         {"であるな"}},
      {{"でござるな"},
         {"でございますね"}},
      {{"ッスね"},
         {"にゃ", "みゃ"}},
   },
   kure = {
      {{"ください", "くださいよ"},
         {"くださいな", "ください"}},
      {{"くれ", "くれよ"},
         {"くれ", "よ"}},
      {{"ね", "よ"},
         {"ね", "ね"}},
      {{"くれ…", "…"},
         {"よ…", "…"}},
      {{"つかわせ", "たもれ"},
         {"つかわせ", "たもれ"}},
      {{"頂きたいでござる"},
         {"くださいませ"}},
      {{"くれッス"},
         {"にゃ", "みゃ"}},
   },
   daro = {
      {{"でしょう"},
         {"でしょう"}},
      {{"だろ"},
         {"だろうね"}},
      {{"だろうね"},
         {"でしょ"}},
      {{"だろ…"},
         {"でしょ…"}},
      {{"であろう"},
         {"であろうな"}},
      {{"でござろうな"},
         {"でございましょう"}},
      {{"ッスね"},
         {"にゃ", "みゃ"}},
   },
   yo = {
      {{"ですよ", "です"},
         {"ですよ", "です"}},
      {{"ぜ", "ぞ"},
         {"わ", "よ"}},
      {{"よ", "ぞ"},
         {"わよ", "わ"}},
      {{"…", "ぞ…"},
         {"わ…", "…"}},
      {{"であろう", "でおじゃる"},
         {"であろうぞ", "でおじゃる"}},
      {{"でござろう"},
         {"でございますわ"}},
      {{"ッスよ", "ッス"},
         {"にゃぁ", "みゃぁ"}},
   },
   aru = {
      {{"あります", "ありますね"},
         {"あります", "ありますわ"}},
      {{"ある", "あるな"},
         {"あるね", "あるよ"}},
      {{"あるね", "あるよ"},
         {"あるわ", "あるわね"}},
      {{"ある…", "あるぞ…"},
         {"あるわ…"}},
      {{"あろう", "おじゃる"},
         {"あろう", "おじゃる"}},
      {{"あるでござる", "あるでござるな"},
         {"ござます"}},
      {{"あるッスよ", "あるッス"},
         {"あにゅ", "あみぅ"}},
   },
   u = {
      {{"います", "いますよ"},
         {"いますわ", "います"}},
      {{"うぜ", "うぞ"},
         {"うわ", "うよ"}},
      {{"うよ", "う"},
         {"うわ", "う"}},
      {{"う…", "うぞ…"},
         {"うわ…", "う…"}},
      {{"うぞよ", "うぞ"},
         {"うぞよ", "うぞ"}},
      {{"うでござる", "うでござるよ"},
         {"うでございます"}},
      {{"うッスよ", "うッス"},
         {"うにぁ", "うみぁ"}},
   },
   na = {
      {{"ですね", "です"},
         {"ですわ", "ですね"}},
      {{"ぜ", "な"},
         {"ね", "な"}},
      {{"ね", "なぁ"},
         {"わ", "わね"}},
      {{"…", "な…"},
         {"…", "わ…"}},
      {{"でおじゃるな", "のう"},
         {"でおじゃるな", "のう"}},
      {{"でござるな"},
         {"でございますわ"}},
      {{"ッスね", "ッス"},
         {"ニァ", "ミァ"}},
   },
   ta = {
      {{"ました", "ましたね"},
         {"ました", "ましたわ"}},
      {{"た", "たな"},
         {"たね", "たよ"}},
      {{"たね", "たよ"},
         {"たよ", "たね"}},
      {{"た…", "たぞ…"},
         {"たわ…"}},
      {{"たぞよ", "たぞな"},
         {"たぞよ"}},
      {{"たでござる"},
         {"ましてございます"}},
      {{"たッスよ", "たッス"},
         {"たにゃぁ", "たみゃぁ"}},
   },
}

for name, list in pairs(endings) do
   I18N.register_function("jp", name, function(chara, mark)
                             -- Lua tables are 1-indexed.
                             if mark == nil then
                                mark = 0
                             end

                             local sex_index
                             if chara.sex == "male" then
                                sex_index = 1
                             else
                                sex_index = 2
                             end

                             local choices = list[chara.talk_type + 1][sex_index]
                             return Rand.choice(choices) .. marks[mark + 1]
   end)
end
