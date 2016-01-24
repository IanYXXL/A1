
<BODY BGCOLOR=white><FONT SIZE=+0>

<H1>mpi++.h
 <FONT SIZE=+1><SUP>[<A HREF='/~magi/opinnot/mpi/src/libsrc/mpi++.h?plain=1'>plain</A>] [<A HREF='/~magi/opinnot/mpi/src/libsrc/mpi++.h?printer=1'>printer</A>]</SUP></FONT>
</H1><PRE>
<FONT COLOR=red>/***************************************************************************
 *   copyright            : (C) 2000 by Marko Grönroos                     *
 *   email                : magi@iki.fi                                    *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
 *
 * Here is defined the mpi++, a C++ interface to MPI.
 *
 **/</FONT>

#ifndef __MPIPP_H__
#define __MPIPP_H__

#include &lt;magic/object.h&gt;
#include &lt;magic/cstring.h&gt;

#include &lt;mpi.h&gt;

class MPIInstance;
class MPIRequest;
class MPIComm;
class MPIDatatype;
class MPIVector;

<FONT COLOR=red>/** A very simple exception class that passes an error message. For
 *  some reason my standard Exception class didn't work with
 *  MPI/MPE. Very strange.
 **/</FONT>
class myexception : public exception {
    const String mMsg;
  public:
    myexception (const char* msg) : mMsg (msg) {}
    const char* what () const {return (CONSTR) mMsg;}
};

<FONT COLOR=red>/** MPI exceptions. */</FONT>
class mpi_error : public myexception {
  public:
    mpi_error (const char* msg) : myexception(msg) {}
};



<FONT COLOR=red>///////////////////////////////////////////////////////////////////////////////</FONT>
<FONT COLOR=red>//        |   | ----  --- ---                                                //</FONT>
<FONT COLOR=red>//        |\ /| |   )  |   |    _    ____  |   ___    _    ___   ___         //</FONT>
<FONT COLOR=red>//        | V | |---   |   |  |/ \  (     -+-  ___| |/ \  |   \ /   )        //</FONT>
<FONT COLOR=red>//        | | | |      |   |  |   |  \__   |  (   | |   | |     |---         //</FONT>
<FONT COLOR=red>//        |   | |     _|_ _|_ |   | ____)   \  \__| |   |  \__/  \__         //</FONT>
<FONT COLOR=red>///////////////////////////////////////////////////////////////////////////////</FONT>

<FONT COLOR=red>/** MPI process data; a singleton (although that is not explicit).
 *
 *  Design Patterns: Singleton.
 **/</FONT>
class MPIInstance : public Object {
  public:
                        MPIInstance     (int& argc, char**& argv);
                        ~MPIInstance    ();

    <FONT COLOR=red>/** Returns a time stamp. */</FONT>
    double              time            () const;

    <FONT COLOR=red>/** Returns the MPI_COMM_WORLD-communicator. */</FONT>
    MPIComm&            world           () {return *mpWorld;}

    <FONT COLOR=red>/** Returns MPI status data block. */</FONT>
    const MPI_Status&   status          () const {return mMPIStatus;}

    <FONT COLOR=red>/** Returns an MPI error code to a string. */</FONT>
    String              error           (int errcode) const;

    <FONT COLOR=red>/** Initializes the buffer to the given length.
     **/</FONT>
    void                initBuffer      (int len);

  protected:
    MPIComm*        mpWorld;
    MPI_Status      mMPIStatus;
    String          mBuffer;

    friend MPIComm;
    friend MPIRequest;
};



<FONT COLOR=red>//////////////////////////////////////////////////////////////////////////////</FONT>
<FONT COLOR=red>//          |   | ----  --- ----                                            //</FONT>
<FONT COLOR=red>//          |\ /| |   )  |  |   )  ___              ___   ____  |           //</FONT>
<FONT COLOR=red>//          | V | |---   |  |---  /   )  __  |   | /   ) (     -+-          //</FONT>
<FONT COLOR=red>//          | | | |      |  | \   |---  /  \ |   | |---   \__   |           //</FONT>
<FONT COLOR=red>//          |   | |     _|_ |  \   \__  \__|  \__!  \__  ____)   \          //</FONT>
<FONT COLOR=red>//                                         |                                //</FONT>
<FONT COLOR=red>//////////////////////////////////////////////////////////////////////////////</FONT>

<FONT COLOR=red>/** Typically a pending non-blocking receive request.
 **/</FONT>
class MPIRequest {
  public:
    <FONT COLOR=red>/** Creates a pending request.
     *
     *  buff A reference to the buffer used for the request. For
     *  example, if the request is a nbRecv, the data will be stored
     *  in this buffer when it arrives.
     **/</FONT>
                    MPIRequest      (MPIComm& comm, String& buff)
                            : mComm (comm), mBuffer (buff) {}

    <FONT COLOR=red>/** Waits until the request has been completed. */</FONT>
    void            wait            ();

    <FONT COLOR=red>/** Checks if the request has been completed, and returns true if
     *  it has, otherwise false.
     **/</FONT>
    bool            check           ();

  protected:
    MPI_Request     mRequest;
    String&         mBuffer;
    MPIComm&        mComm;

    void            complete        ();

    friend MPIComm;
};



<FONT COLOR=red>//////////////////////////////////////////////////////////////////////////////</FONT>
<FONT COLOR=red>//                  |   | ----  ---  ___                                    //</FONT>
<FONT COLOR=red>//                  |\ /| |   )  |  /   \                                   //</FONT>
<FONT COLOR=red>//                  | V | |---   |  |      __  |/|/| |/|/|                  //</FONT>
<FONT COLOR=red>//                  | | | |      |  |     /  \ | | | | | |                  //</FONT>
<FONT COLOR=red>//                  |   | |     _|_ \___/ \__/ | | | | | |                  //</FONT>
<FONT COLOR=red>//////////////////////////////////////////////////////////////////////////////</FONT>

<FONT COLOR=red>/** An MPI communicator object.
 **/</FONT>
class MPIComm : public Object {
  public:

    <FONT COLOR=red>/** Creates a communications channel with the given comm tag.
     **/</FONT>
                    MPIComm         (MPIInstance& mpi, int commtag)
                            : mMPI (mpi), mCommTag (commtag) {}

    <FONT COLOR=red>/** Sends a message. Blocking. */</FONT>
    void            send            (void* buffer, int len, MPI_Datatype datatype, int receiver);

    <FONT COLOR=red>/** Sends a string buffer. Blocking. */</FONT>
    void            send            (const String& buffer, int receiver);

    <FONT COLOR=red>/** Sends a message. Blocking. */</FONT>
    void            nbSend          (void* buffer, int len, MPI_Datatype datatype, int receiver);

    <FONT COLOR=red>/** Sends a string buffer. Non-blocking. */</FONT>
    void            nbSend          (const String& buffer, int receiver);

    <FONT COLOR=red>/** Receives a message. Blocking. */</FONT>
    int             recv            (void* buffer, int maxlen, MPI_Datatype datatype, int source);

    <FONT COLOR=red>/** Receives a string buffer. Blocking. */</FONT>
    void            recv            (String& buffer, int maxlen, int sender);

    <FONT COLOR=red>/** Receives a message. Non-blocking.
     *
     *  Returns an MPIRequest object that can be used to wait()
     *  or check() if the message has yet been received.
     **/</FONT>
    MPIRequest*     nbRecv          (String& buffer, int maxlen, int sender);

    <FONT COLOR=red>/** Coating for the other recv. */</FONT>
    String          recv            (int maxlen, int sender);

    <FONT COLOR=red>/** Performs an operation with all processors. */</FONT>
    void            allReduce       (const void* sendBuffer, void* recvBuffer, int count, const MPI_Datatype& datatype, const MPI_Op& op);

    <FONT COLOR=red>/** Blocks the calling process until all processes in the comm
     *  group have called this method.
     *
     *  Involves no data transmission.
     **/</FONT>
    void            barrier         ();

    <FONT COLOR=red>/** Returns the MPI process rank of the current process. */</FONT>
    int             getRank         () const;

    <FONT COLOR=red>/** Returns the COMM tag. */</FONT>
    int             getCommTag      () const {return mCommTag;}

    <FONT COLOR=red>/** Returns the number of processor in the given comm channel */</FONT>
    int             size            () const;

    <FONT COLOR=red>/** Returns the (well, singleton) MPI object. */</FONT>
    MPIInstance&    mpi             () {return mMPI;}

  protected:
    MPIInstance&    mMPI;
    int             mCommTag;
};



<FONT COLOR=red>//////////////////////////////////////////////////////////////////////////////</FONT>
<FONT COLOR=red>//        |   | ----  --- ___                                               //</FONT>
<FONT COLOR=red>//        |\ /| |   )  |  |  \   ___   |   ___   |         --   ___         //</FONT>
<FONT COLOR=red>//        | V | |---   |  |   |  ___| -+-  ___| -+- \   | |  ) /   )        //</FONT>
<FONT COLOR=red>//        | | | |      |  |   | (   |  |  (   |  |   \  | |--  |---         //</FONT>
<FONT COLOR=red>//        |   | |     _|_ |__/   \__|   \  \__|   \   \_/ |     \__         //</FONT>
<FONT COLOR=red>//                                                   \_/                    //</FONT>
<FONT COLOR=red>//////////////////////////////////////////////////////////////////////////////</FONT>

class MPIDatatype : public Object {
  public:
    const MPI_Datatype& getType     () const {return mDatatype;}

  protected:
    MPIDatatype () {}
    MPI_Datatype    mDatatype;
  private:
};

class MPIVector : public MPIDatatype {
  public:
                MPIVector   () {}
                MPIVector   (int count, int blocklength, int stride, MPI_Datatype oldtype);
    void        make        (int count, int blocklength, int stride, MPI_Datatype oldtype);

  private:
};

#endif
</B></PRE>
<P><IMG SRC=/~magi/bottline.gif><BR>
<A HREF=/~magi/><IMG SRC=/~magi/home.gif ALT='Pääsivu' BORDER=0 WIDTH=30 HEIGHT=30></A> <A HREF=../><IMG SRC=/~magi/up.gif ALT="Takaisin" BORDER=0 WIDTH=30 HEIGHT=30></A>
<FONT SIZE=-1>Generated with (<A HREF=http://www.python.org/>Python</A> version) of tab4.cgi by <A HREF='http://www.iki.fi/magi/'>Marko Grönroos</A> (<A HREF='mailto:magi@iki.fi'>magi@iki.fi</A>), 1999
  </body></html>
