#include "TestData.hpp"

const std::vector<User> TestData::user = {
  {"莊漢生", "chonghongsan@example.com", "123456", "莊漢生自我介紹", "www.google.com"},
  {"文翠珊", "mantsuishan@example.com", "654321", "文翠珊自我介紹", "www.bing.com"},
  {"卓慧思", "chuekwaisze@example.com", "abcdef", "卓慧思自我介紹", "www.yahoo.com"},
  {"辛偉誠", "", "", "", ""},
};

const std::vector<Article> TestData::article = {
  {"想節能,輪胎能!", "優科豪馬輪胎", "只要換上新開發的節能輪胎,同樣多的油量,卻能比以前跑得更遠,想節能,輪胎能!優科豪馬輪胎", {0, 1, 2}},
  {"金坷垃", "這裡是全球矚目的焦點", "金坷垃,80種稀缺礦物質提煉的精華金坷垃,大型電腦定位生產,30000攝氏度精密提煉!", {3, 4, 5, 6}},
  {"即刻轉身射個三分波", "電視節目有好多種", "哎呀,等我變身先!我覺得呢個人相當有嫌疑.即刻轉身射個三分波啊,又反超前一分喇!", {7, 8}},
};

const std::vector<std::string> TestData::tag = {
  "節能", "輪胎", "優科豪馬輪胎",
  "金坷垃", "全球矚目", "精華", "大型電腦",
  "三分波", "電視節目"
};

const std::vector<std::string> TestData::comment = {
  "雞蛋六隻",
  "糖呢就兩茶匙",
  "仲有啲橙皮添",
  "夏天為大地帶來雨水",
  "渠道暢通,有你幫手,必定會成功",
  "渠務投訴熱線 2300 1110"
};