#include <iostream>
#include <curl/curl.h>
#include <string>
#include <iomanip>
#include <optional>
#include <algorithm>
#include "includes/json.hpp"
#include "includes/decimal.h"

using json = nlohmann::json;

struct asset {
  std::string id;
  dec::decimal<2> price;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

json fetch(const char * url) {
  auto curl = curl_easy_init();
  CURLcode res;
  std::string read_buffer;

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
      json data = json::parse(read_buffer).at("data");
      return data;
    }
    
    curl_easy_cleanup(curl);
  }
}

std::optional<std::string> lookup_id(const json &cache, const std::string &symbol) {
  for (auto it : cache) {
    auto data = it;
    if (data.at("symbol") == symbol) {
      return data.at("id");
    }
  }

  return std::nullopt;
}

// TODO: need better decimal library
void lookup_and_update_price(asset &asset) {
  auto url = "https://api.coincap.io/v2/assets/" + asset.id;
  json res = fetch(url.c_str());
  std::string price_str = res.at("priceUsd");
  auto price = std::stold(price_str); // long double is platform dependent
  dec::decimal<2> dec_price{price}; // parses total 18 digits. the price string on api is 20 digits which overflows an integer perhaps
  asset.price = dec_price;
}

int main(int argc, char** argv) {
  std::string input = "";
  dec::decimal_format format{'.', ','};
  std::string url = "https://api.coincap.io/v2/assets/";
  std::cout << "Fetching all crypto assets..." << std::endl;
  json assets_cache = fetch(url.c_str());

  while(input != "exit") {
    std::cout << "[KRYPT]: ";
    std::cin >> input;
    

    std::transform(input.begin(), input.end(), input.begin(),
    [](auto input) {return std::toupper(input); });

    asset current_asset;
    auto id = lookup_id(assets_cache, input);

    if(id.has_value()) { 
      current_asset.id = id.value();
    } else {
      std::cout << "Symbol not found" << std::endl;
      continue;
    }

    lookup_and_update_price(current_asset);
    std::cout << dec::toString(current_asset.price, format) << "\n";
  }

  return 0;
}
