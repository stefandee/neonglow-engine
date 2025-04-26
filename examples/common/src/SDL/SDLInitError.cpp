#include "SDLInitError.h"
#include <SDL2\SDL.h>

SDLInitError::SDLInitError() :
    exception(),
    msg( SDL_GetError() )
{
}

SDLInitError::SDLInitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

SDLInitError::~SDLInitError() throw()
{
}

const char * SDLInitError::what() const throw()
{
    return msg.c_str();
}

