/**************************************************************************
**       Title: basic exception
**    $RCSfile: CmdLineError.cc,v $
**   $Revision: 1.1 $$Name:  $
**       $Date: 2002/03/26 07:36:28 $
**   Copyright: GPL $Author: ronneber $
** Description:
**
**    
**
**-------------------------------------------------------------------------
**
**  $Log: CmdLineError.cc,v $
**  Revision 1.1  2002/03/26 07:36:28  ronneber
**  restructuring for autoconf
**
**  Revision 1.1.1.1  2002/03/22 13:45:07  pigorsch
**  moved from polsoft repository
**
**
**
**************************************************************************/

#include "CmdLineError.hh"

CmdLineError::
CmdLineError()
{};

CmdLineError::
CmdLineError(const CmdLineError& copy)
         :pMessage(copy.str())
{};
