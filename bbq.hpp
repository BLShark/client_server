#pragma once

#include <vector>
#include <list>
#include <array>
#include <map>
#include <functional>
#include <algorithm>
#include <stdlib.h>     // srand, rand
#include <time.h>       //time
#include <chrono>       // std::chrono::seconds

class BBQ
{
    using func_req = std::function<void(std::string& answer)>;

    enum class state : uint8_t {
        NEW_CLIENT = 0,
        COOKING,
        SERVED,
        CLOSED
    };
    enum class string_code : uint8_t {
        HUNGRY = 0,
        I_TAKE,
        NO_THANKS,
        WRONG_REQ
    };

    string_code string_to_code(std::string& s) {
        std::for_each(s.begin(), s.end(), [](char &c) { c = ::toupper(c); });
        if (s.compare("I AM HUNGRY") == 0) return string_code::HUNGRY;
        else if (s.compare("I TAKE THAT") == 0) return string_code::I_TAKE;
        else if (s.compare("NO THANKS") == 0) return string_code::NO_THANKS;
        else return string_code::WRONG_REQ;
    }


    std::vector<std::string> _meals = {"CHICKEN READY", "BEEF READY", "LAST MONTH MAMMOTH READY"};
    state m_currState = state::NEW_CLIENT;
    func_req m_callBackWhenReady;

    std::string _waiting() {
        m_currState = state::COOKING;
        std::thread cheef(&BBQ::StartCooking, this);
        cheef.detach();
        return "OK WAIT!";
    }
    std::string _closed() {
        m_currState = state::CLOSED;
        return  "CLOSED BYE!";
    }

    std::string _served() {
        m_currState = state::SERVED;
        return "SERVED BYE!";
    }

    void StartCooking(){
        srand(time(NULL));
        auto select = rand() % 2 + 0;
        std::this_thread::sleep_for (std::chrono::seconds(5));
        m_callBackWhenReady(_meals.at(select));
    }
public:
    BBQ(func_req ready_req): m_callBackWhenReady(ready_req){}
    ~BBQ(){}

    std::string Request(std::string &&order)
    {
        switch (string_to_code(order))
        {
            case string_code::HUNGRY:
            {
               return _waiting();
            }
            break;
            case string_code::I_TAKE:
            {
               return _served();
            }
            break;
            case string_code::NO_THANKS:
            {
                return _waiting();
            }
            break;
            default: return "TRY AGAIN!";
            break;
        }
    }
};

