#include "get-translate.h"
#include "get-config.cpp"
#include <cpr/api.h>
#include <cpr/cpr.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <regex>


void prepareQuery(std::string &query) {
  // NAME does not need translation
  auto PosNewLine = std::distance(query.begin(), std::find(query.begin(), query.end(), '\n'));
  std::cout << BOLDCYAN << query.substr(0, PosNewLine) << RESET << ": ";

  query = query.substr(PosNewLine+1, query.size());
  std::replace(query.begin(), query.end(), ' ', '+');
  std::replace(query.begin(), query.end(), '\n', '+');
  std::replace(query.begin(), query.end(), '|', 'I');
  query = std::regex_replace(query, std::regex("\\+\\+"), "+");

  if (query.back() == '+') {
    query.pop_back();
  }
}

std::string gglTranslate(std::string &query, Config &config) {
  if (config.needDemo) {
    std::cout << "starting get-request with query:\n" << query << std::endl;
  }
  cpr::Response r = cpr::Get(cpr::Url{ "https://translate.google.com/m?sl=" + config.translateLangFrom +
                                       "&tl=" + config.translateLangTo + "&hl=" + config.translateLangTo + "&q=" + query},
                             cpr::Header{ { "User-Agent",
                                            "Opera/9.80 (Android; Opera Mini/11.0.1912/37.7549; U; pl) Presto/2.12.423 Version/12.16" },
                                          { "Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8" },
                                          { "Accept-Language", "en-US,en;q=0.5" },
                                          { "DNT", "1" },
                                          { "Upgrade-Insecure-Requests", "1" },
                                          { "Cache-Control", "no-cache" },
                                          { "Pragma", "no-cache" },
                                        } );
  if(r.status_code == 200) {
    std::string::size_type res_start = r.text.find(R"(<div class="result-container">)") + 30;
    std::string::size_type res_stop  = r.text.find(R"(</div>)", res_start);
    return r.text.substr(res_start, res_stop - res_start);
  }
  if(config.needDemo) {
    std::cout << "get-request returns " << r.status_code << " on this query \n" << query << std::endl;
  }

  return "error";
}



void translate(std::string &query, std::string &queryPrevious, Config &config) {
  if (query != queryPrevious && !query.empty()) {
    queryPrevious = query;

    if (config.needDemo) { std::cout << query << std::endl; }

  } else {
    return;
  }

  prepareQuery(query);
  if (config.translator == "ggl") {
    std::cout << gglTranslate(query, config) << '\n'<< std::endl;
  }
}
