#ifndef SDLINITERROR_H
#define SDLINITERROR_H

#include <string>
#include <exception>

class SDLInitError : public std::exception
{
    std::string msg;
public:
    SDLInitError();
    SDLInitError( const std::string & );
    virtual ~SDLInitError() throw();
    virtual const char * what() const throw();
};

#endif // SDLINITERROR_H
