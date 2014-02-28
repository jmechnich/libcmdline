/**************************************************************************
**       Title: test program for libCmdLine that uses LMBUNIT
**    $RCSfile: testCmdLine.cc,v $
**   $Revision: 1.2 $$Name:  $
**       $Date: 2002/04/11 08:45:14 $
**   Copyright: GPL $Author: ronneber $
** Description:
**
**    
**
**-------------------------------------------------------------------------
**
**  $Log: testCmdLine.cc,v $
**  Revision 1.2  2002/04/11 08:45:14  ronneber
**  - adapted to new position of lmbunit.h
**  - added check for new Compiler<float>
**
**  Revision 1.1  2002/04/04 12:32:45  pigorsch
**  - initial revision
**
**
**
**************************************************************************/

#include "lmbunit.hh"

#include <ArgvIter.hh>
#include <CmdLine.hh>
#include <CmdArgs.hh>
#include <Compiler.hh>
#include <Errors.hh>

static void testConstructorDefaultHelp()
{
  CmdLine cmd("test", "test program");
  char* argv="--help";
  ArgvIter ai(1, &argv);
  try
  {
    cmd.parse(ai);
    LMBUNIT_WRITE_FAILURE("expected excetion 'CmdLineUsageError'");
  }
  catch (CmdLineUsageError e)
  {
    return;
  }
}

static void testConstructorDefaultUsageHTML()
{
  CmdLine cmd("test", "test program");
  char* argv="--usagehtml";
  ArgvIter ai(1, &argv);
  try
  {
    cmd.parse(ai);
    LMBUNIT_WRITE_FAILURE("expected excetion 'CmdLineUsageHTMLError'");
  }
  catch (CmdLineUsageHTMLError e)
  {
    return;
  }
}

static void testConstructorDirectAppending()
{
  CmdArgType<int> arg1('a', "arg1", "value", "this is arg1");
  CmdArgType<int> arg2('b', "arg2", "value", "this is arg2");
  CmdLine cmd("test", "test program", true, &arg1, &arg2, 0);
    
  const char* a1="--arg1";
  const char* a2="42";
  const char* a3="-b";
  const char* a4="17";
  const char* const argv[]={a1, a2, a3, a4};

  ArgvIter ai(4, argv);
  cmd.parse(ai);
  return;
}

static void testDuplicateArgs()
{
  CmdArgType<int> arg1('a', "arg1", "value", "this is arg1");
  CmdArgType<int> arg2('b', "Arg1", "value", "this is arg2");
  try
  {
    CmdLine cmd("test", "test program", true, &arg1, &arg2, 0);
    LMBUNIT_WRITE_FAILURE("expected excetion 'CmdLineDublicateArgError'");
  }
  catch (CmdLineDublicateArgError e)
  {
    return;
  }
}

static void testFloatArg()
{
  CmdArgType<float> miau('p', "pi", "value", 
                         "give your own definition of pi");
  
  CmdLine cmd("test", "test program", true, &miau, NULL);
  

  const char* argv[] = {"-p", "3.1415"};
  ArgvIter ai( 2, argv);
  cmd.parse( ai);
  
  LMBUNIT_ASSERT_EQUAL_DELTA( miau.value(), 3.1415, 0.0000001);
}



int main()
{
  LMBUNIT_WRITE_HEADER();
  
  LMBUNIT_RUN_TEST(testConstructorDefaultHelp());
  LMBUNIT_RUN_TEST(testConstructorDefaultUsageHTML());
  LMBUNIT_RUN_TEST(testConstructorDirectAppending());
  LMBUNIT_RUN_TEST(testDuplicateArgs());
  LMBUNIT_RUN_TEST(testFloatArg());

  LMBUNIT_WRITE_STATISTICS();
  
  return _nFails;
}

