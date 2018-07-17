#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

enum class us_denomination_type_t : uint8_t {
    penny,
    nickle,
    dime,
    quarter,
    dollar,
    two_dollar,
    five_dollar,
    ten_dollar,
    twenty_dollar,
    fifty_dollar,
    one_hundred_dollar
};

struct us_denomination_name_t {
    std::string singular {};
    std::string plural {};
};

static std::unordered_map<us_denomination_type_t, us_denomination_name_t> s_denomination_names = {
    {us_denomination_type_t::penny,              {"penny", "pennies"}},
    {us_denomination_type_t::nickle,             {"nickle", "nickle"}},
    {us_denomination_type_t::dime,               {"dime", "dimes"}},
    {us_denomination_type_t::quarter,            {"quarter", "quarters"}},
    {us_denomination_type_t::dollar,             {"dollar bill", "dollar bills"}},
    {us_denomination_type_t::two_dollar,         {"two dollar bill", "two dollar bills"}},
    {us_denomination_type_t::five_dollar,        {"five dollar bill", "five dollar bills"}},
    {us_denomination_type_t::ten_dollar,         {"ten dollar bill", "ten dollar bills"}},
    {us_denomination_type_t::twenty_dollar,      {"twenty dollar bill", "twenty dollar bills"}},
    {us_denomination_type_t::fifty_dollar,       {"fifty dollar bill", "fifty dollar bills"}},
    {us_denomination_type_t::one_hundred_dollar, {"one hundred dollar bill", "one hundred dollar bills"}},
};

static us_denomination_name_t* denomination_name(us_denomination_type_t type) {
    auto it = s_denomination_names.find(type);
    if (it == s_denomination_names.end())
        return nullptr;
    return &it->second;
}

struct us_denomination_t {
    us_denomination_type_t type;
    uint32_t number_of_pennies = 0;
};

static std::map<us_denomination_type_t, us_denomination_t> s_denominations = {
    {us_denomination_type_t::penny,              {us_denomination_type_t::penny, 1}},
    {us_denomination_type_t::nickle,             {us_denomination_type_t::nickle, 5}},
    {us_denomination_type_t::dime,               {us_denomination_type_t::dime, 10}},
    {us_denomination_type_t::quarter,            {us_denomination_type_t::quarter, 25}},
    {us_denomination_type_t::dollar,             {us_denomination_type_t::dollar, 100}},
    {us_denomination_type_t::two_dollar,         {us_denomination_type_t::two_dollar, 200}},
    {us_denomination_type_t::five_dollar,        {us_denomination_type_t::five_dollar, 500}},
    {us_denomination_type_t::ten_dollar,         {us_denomination_type_t::ten_dollar, 1000}},
    {us_denomination_type_t::twenty_dollar,      {us_denomination_type_t::twenty_dollar, 2000}},
    {us_denomination_type_t::fifty_dollar,       {us_denomination_type_t::fifty_dollar, 5000}},
    {us_denomination_type_t::one_hundred_dollar, {us_denomination_type_t::one_hundred_dollar, 10000}},
};

static us_denomination_t* denomination(us_denomination_type_t type) {
    auto it = s_denominations.find(type);
    if (it == s_denominations.end())
        return nullptr;
    return &it->second;
}

struct amount_t {
    static amount_t from_amount(const std::string& amount) {
        std::string whole {};
        std::string fractional {};
        bool in_fractional_part = false;

        for (const auto& c : amount) {
            if (isdigit(c)) {
                if (in_fractional_part) {
                    fractional += c;
                } else {
                    whole += c;
                }
            } else if (c == '.') {
                in_fractional_part = true;
            }
        }

        auto dollar = denomination(us_denomination_type_t::dollar);

        uint32_t whole_amount = static_cast<uint32_t>(std::atoi(whole.c_str()));
        uint32_t fractional_amount = static_cast<uint32_t>(std::atoi(fractional.c_str()));
        uint32_t pennies = fractional_amount;
        pennies += whole_amount * dollar->number_of_pennies;

        return amount_t(pennies);
    }

    amount_t(uint32_t pennies): pennies(pennies) {
    }

    amount_t operator- (uint32_t pennies) {
        this->pennies -= pennies;
        return *this;
    }

    amount_t operator- (const amount_t& rhs) const {
        return amount_t(pennies - rhs.pennies);
    }

    amount_t operator+ (const amount_t& rhs) const {
        return amount_t(pennies + rhs.pennies);
    }

    size_t denominations_present(us_denomination_type_t type) {
        auto denom = denomination(type);
        return pennies / denom->number_of_pennies;
    }

    uint32_t pennies = 0;
};

struct denomination_count_t {
    size_t count;
    us_denomination_type_t type;
};

int main() {
    auto receipt_amount = amount_t::from_amount("12.67");
    auto tendered_amount = amount_t::from_amount("20.00");
    auto change_amount = tendered_amount - receipt_amount;

    std::vector<denomination_count_t> denomination_counts {};

    for (auto it = s_denominations.rbegin(); it != s_denominations.rend(); ++it) {
        auto kvp = *it;
        auto count = change_amount.denominations_present(kvp.first);
        if (count > 0) {
            denomination_counts.push_back(denomination_count_t {
                .count = count,
                .type = kvp.first,
            });
        }
        change_amount - (count * kvp.second.number_of_pennies);
    }

    for (const auto& denom_count : denomination_counts) {
        auto denom_name = denomination_name(denom_count.type);
        std::cout << denom_count.count << " ";

        if (denom_count.count == 1)
            std::cout << denom_name->singular;
        else if (denom_count.count > 1)
            std::cout << denom_name->plural;

        std::cout << std::endl;
    }

    return 0;
}