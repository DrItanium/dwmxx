#ifndef UTIL_H__
#define UTIL_H__
#include <string>
#include <sstream>
namespace dwm
{
template<typename ... Args>
void print(std::ostream& os, Args&& ... params) 
{ 
    (os << ... << params);
}

class Exception : public std::exception
{
public:
    template<typename ... Args>
    explicit Exception(Args&& ... args) noexcept 
    {
        std::ostringstream os;
        print(os, args...);
        _message = os.str();
    }
    ~Exception() = default;
    auto getMessage() const { return _message; }
    virtual const char* what() const noexcept final;
    Exception& operator=(const Exception&) = delete;
private:
    std::string _message;

};
} // end namespace dwm
#endif // end UTIL_H__
