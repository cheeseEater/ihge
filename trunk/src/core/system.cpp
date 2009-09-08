/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core system functions
*/


#include "hge_impl.h"

#include <string.h>

#if !HGE_IPHONE && WIN32

#include "system_win32.inc"

#else

#include "system_iphone.inc"

#endif //!HGE_IPHONE && WIN32